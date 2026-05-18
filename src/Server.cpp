#include "Server.hpp"

Server::Server() : _port_s(0),_password(""),_server_socket(-1), _serverName(""), _epoll_fd(-1) {
    std::memset(&this->_server_address, 0, sizeof(this->_server_address));
}

Server &Server::operator=(const Server &orignal) {
    if (this != &orignal){} return *this; 
}

Server::Server(const Server &to_copy) {
    *this = to_copy;
}

void Server::setServerName(const char *sname) {
    this->_serverName = sname;
}

Server::Server(int port, std::string &password,const char *serverN) {
    setServerName(serverN);
    setPort(port);
    setPass(password);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        throw std::runtime_error("Error socket");
    setServer_socket(server_socket);
    if (setNonBlocking_socket(this->getServer_socket()) == -1)
    {
        close(server_socket);
        throw std::runtime_error("Error fcntl");
    }
    
    _commands["PASS"] = new Pass();
    _commands["USER"] = new User();    
    _commands["JOIN"] = new Join();
    _commands["NICK"] = new Nick();
    _commands["PART"] = new Part();
    _commands["QUIT"] = new Quit();
    _commands["PRIVMSG"] = new PrivMsg();
    _commands["KICK"] = new Kick();
    _commands["INVITE"] = new Invite();
    _commands["TOPIC"] = new Topic();
    _commands["MODE"] = new Mode();
}

Server::~Server() {
    if (_server_socket != -1)
        close(_server_socket);

    std::map<std::string, Command*>::iterator it;
    for (it = _commands.begin(); it != _commands.end(); ++it){
        delete it->second;
    }
    close (_server_socket);
}

void Server::addClient(int fd, Client *client) {
    if (fd != client->getFd()) {
        std::cout << "ERROR: fd mismatch" << std::endl;
        return;
    }
    _clients[fd] = client;
}

int Server::setNonBlocking_socket(int socket_s) {
    while (fcntl(socket_s, F_SETFL, O_NONBLOCK) == -1) {
        if (errno != EINTR)
            return -1;
    }
    return 1;
}

void Server::setSockectReusable() {
    int opt = 1;
    setsockopt(getServer_socket(), SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));
}

int Server::bindSocketToServer() {
    if (bind(getServer_socket(), (struct sockaddr *)&getServer_address(), sizeof(getServer_address())) < 0) {
        close(getServer_socket());
        setServer_socket(-1);
        throw Server::Error_fd();
    }
    return 1;
}

int Server::listenServer() {
   if (listen(getServer_socket(), MAX_CONECTIONS) < 0) {
        close(getServer_socket());
        setServer_socket(-1);
        throw Server::Error_fd();
    }
    return 1;
}

void Server::handleClientData(Client& client, const std::string& tempBuffer, Parser& parser){
    client.setMesagge(client.getMessage() + tempBuffer);
    std::size_t pos = 0;
    std::string currentBuffer = client.getMessage();

    while((pos = currentBuffer.find("\r\n")) != std::string::npos){
        std::string command = currentBuffer.substr(0, pos);
        currentBuffer.erase(0, pos + 2);
        std::cout << "message: " << command << "$" << std::endl;
        parser.parseMessage(client, command, *this);
    }
    client.setMesagge(currentBuffer);
}

void Server::executeCommand(Client& client, IrcMessage& message){
    std::map<std::string, Command*>::const_iterator it = _commands.find(message.command);
    if (it != _commands.end()){
        it->second->execute(client, message.params, *this);
    }
}

void Server::setPort(int port) { this->_port_s = port; }
void Server::setPass(std::string pass) { this->_password = pass; }
void Server::setServer_socket(int socket) { this->_server_socket = socket; }
void Server::setServer_address() {
    this->_server_address.sin_family = AF_INET;
    this->_server_address.sin_addr.s_addr=INADDR_ANY;
    this->_server_address.sin_port = htons(getPort());
}
void Server::setIP(char *ip) { _ip = ip;}

std::string Server::getIP() const { return (_ip);}
std::string Server::getPass()const { return _password; }
int Server::getPort() const{return _port_s;}
int Server::getServer_socket() const{return _server_socket;}
struct sockaddr_in& Server::getServer_address(){ return _server_address;}
std::string Server::getServerName(){ return _serverName;}
std::map<int, Client *>& Server::getClients(){ return _clients;}
const std::map<std::string, Command*>& Server::getCommands() const{ return _commands;}
void Server::setEpoll_fd(int epoll_fd) { this->_epoll_fd = epoll_fd; }
int Server::getEpoll_fd() const { return this->_epoll_fd; }

void Server::enableSendEvent(int epoll_fd, int fd_client) {
    struct epoll_event epoll_v ;
    epoll_v.events = EPOLLIN | EPOLLOUT;
    epoll_v.data.fd = fd_client;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,fd_client,&epoll_v) < 0)
        std::cerr << "Error on SendEvent" << std::endl; 
}

void Server::start() {
    std::memset(&_server_address, 0, sizeof(_server_address));
    setServer_address();
    setSockectReusable();    
    bindSocketToServer();
    listenServer();
    ft_message("Listening ...", NULL, 1);    
    
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd < 0) {
        throw std::runtime_error("Error in epoll_create1");
    }
    
    struct epoll_event event_epoll;
    event_epoll.events = EPOLLIN;
    event_epoll.data.fd = _server_socket;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_socket, &event_epoll) < 0) {
        throw std::runtime_error("Error in epoll_ctl");
    }
}

void Server::disconnectClient(int client_fd) {
    std::map<int, Client*>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        delete it->second;
        _clients.erase(it);
        close(client_fd);
    }
}

void Server::handleNewConnection() {
    Client *newClient = new Client();
    newClient->getAddressLen() = sizeof(struct sockaddr_in);
    int new_socket = accept(_server_socket, (struct sockaddr *)&newClient->getClient_addres(), &newClient->getAddressLen());
    
    if (new_socket < 0) { 
        delete newClient;
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Error in accept" << std::endl;
        }
        return;
    }
    
    newClient->setFd(new_socket);
    if (setNonBlocking_socket(new_socket) == -1) {
        std::cerr << "Error fcntl on new socket" << std::endl;
        delete newClient;
        close(new_socket);
        return;
    }
    
    addClient(new_socket, newClient);
    
    struct epoll_event new_event_c;
    new_event_c.events = EPOLLIN;
    new_event_c.data.fd = new_socket;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, new_socket, &new_event_c);
    std::cout << "New client connected." << std::endl;
}

void Server::handleClientActivity(int client_fd, uint32_t events, Parser& parser) {
    std::map<int, Client*>::iterator it = _clients.find(client_fd);
    if (it == _clients.end()) return;
    Client* client = it->second;

    if (events & EPOLLIN) {
        char buffer[1024];
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes <= 0) {
            if (bytes == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
                std::cout << YELLOW << "Client disconnected." << RESET << std::endl;
                disconnectClient(client_fd);
            }
            return;
        }
        
        buffer[bytes] = '\0';
        std::cout << GREEN << "Received: " << buffer << RESET << std::endl;
        handleClientData(*client, std::string(buffer), parser);
        
        if (!client->getSendBuff().empty()) {
            enableSendEvent(_epoll_fd, client_fd);
        }
    }
    
    if (events & EPOLLOUT) {
        std::string& sendBuffer = client->getSendBuff();
        if (!sendBuffer.empty()) {
            int sent = send(client_fd, sendBuffer.c_str(), sendBuffer.size(), 0);
            if (sent < 0) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    std::cout << YELLOW << "Error sending, client disconnected" << RESET << std::endl;
                    disconnectClient(client_fd);
                }
                return;
            }
            client->clearSendingBugg();
        }
        
        if (client->getSendBuff().empty()) {
            struct epoll_event ev;
            ev.events = EPOLLIN;
            ev.data.fd = client_fd;
            epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, client_fd, &ev);
        }
    }
}

void Server::run() {
    Parser parser;
    struct epoll_event events_epoll[10];

    while (true) { 
        int num_events = epoll_wait(_epoll_fd, events_epoll, 10, -1);
        
        if (num_events < 0) {
            if (errno == EINTR) continue;
            std::cerr << "Error in epoll_wait" << std::endl;
            break;
        }
        
        for (int i = 0; i < num_events; i++) {
            if (events_epoll[i].data.fd == _server_socket) {
                handleNewConnection();
            } else {
                handleClientActivity(events_epoll[i].data.fd, events_epoll[i].events, parser);
            }
        }
    }
}
