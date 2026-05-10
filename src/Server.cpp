#include "Server.hpp"

Server::Server() : _port_s(0),_password(""),_server_socket(-1), _serverName(""), _epoll_fd(-1)
{
    std::memset(&this->_server_address, 0, sizeof(this->_server_address));
    std::cout << "no deberias estar haciendo esto;" << std::endl;
}

Server &Server::operator=(const Server &orignal)
{
    if (this != &orignal){std::cout << "No de deberias estar aqui\n" ;} return *this;
}

Server::Server(const Server &to_copy)
{
    *this = to_copy;
}

void Server::setServerName(const char *sname)
{
    this->_serverName = sname;
}

Server::Server(int port, std::string &password,const char *serverN)
{
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
	//_commands["USER"] = new User(); duplicado
    _commands["KICK"] = new Kick();
    _commands["INVITE"] = new Invite();
    _commands["TOPIC"] = new Topic();
    _commands["MODE"] = new Mode();


}


Server::~Server()
{
    if (_server_socket != -1)
        close(_server_socket);

    std::map<std::string, Command*>::iterator it;
    for (it = _commands.begin(); it != _commands.end(); ++it){
        delete it->second;
    }
    // cerrar los fd de los clientes tambien
    close (_server_socket);
}

void Server::addClient(int fd, Client *client)
{
	//se modifico por que no conectaba los clientes.
//	_clients[fd] = client;
	if (fd != client->getFd())
	{
		std::cout << "ERROR: fd mismatch" << std::endl;
		return;
	}
	_clients[fd] = client;
/*    cliente->setFd(fd);
    _clients.insert(std::pair<int, Client *>(fd, cliente));*/
//	std::cout << "esntro aqui" << std::endl;
}

int Server::setNonBlocking_socket(int socket_s)
{
    int flags; 
    while ((flags = fcntl(socket_s, F_GETFL, 0)) == -1)
    {
        if (errno != EINTR)
            return -1;
    }
    while (fcntl(socket_s, F_SETFL, flags | O_NONBLOCK) == -1) {
        if (errno != EINTR)
            return -1;
    }
    return 1;
}
void Server::setSockectReusable()
{
    int opt = 1;
    setsockopt(getServer_socket(), SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));
}


int Server::bindSocketToServer()
{
    if (bind(getServer_socket(), (struct sockaddr *)&getServer_address(), sizeof(getServer_address())) < 0)
    {
       // close()  fdsc
        close(getServer_socket());
        setServer_socket(-1);
        throw Server::Error_fd();
        //return -1;
    }
    return 1;
}
int Server::listenServer()
{
   if (listen(getServer_socket(), MAX_CONECTIONS) < 0)
    {
        //closee   
        close(getServer_socket());
        setServer_socket(-1);
        throw Server::Error_fd();
       // return -1;
    }
    return 1;
}
// ya no lo necestiamos
// int Server::sendhandshake(int client_fd)
// {
// 	//aqui modificamos la llamada desde el main esta comentada
// 	//linea 138
//     return (send(client_fd, "\n",2 , 0));
// }

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

//  void Server::setEpoll()
//  {
//     this->epoll_fd = epoll_create1(0);
//     if (this->epoll_fd < 0)
//     {
//         throw("Error Epoll");
//         close(this->getServer_socket());
//     }
//     this->s_event_epoll.events = EPOLLIN | EPOLLET;
//     this->s_event_epoll.data.fd = this->getServer_socket();
//     if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->getServer_socket(), &s_event_epoll)< 0)
//     {
//         throw("Error Epoll_ctl");
//         close(this->getServer_socket());
//     }
//  }

void Server::setPort(int port)
{
    this->_port_s = port;
}
void Server::setPass(std::string pass)
{
    this->_password = pass;
}
void Server::setServer_socket(int socket)
{
    this->_server_socket = socket;
}
void Server::setServer_address()
{
    this->_server_address.sin_family = AF_INET;
    this->_server_address.sin_addr.s_addr=INADDR_ANY;
    this->_server_address.sin_port = htons(getPort());
}
std::string Server::getPass()const { return _password; }
int Server::getPort() const{return _port_s;}
int Server::getServer_socket() const{return _server_socket;}
struct sockaddr_in& Server::getServer_address(){ return _server_address;}
std::string Server::getServerName(){ return _serverName;}
std::map<int, Client *>& Server::getClients(){ return _clients;}
const std::map<std::string, Command*>& Server::getCommands() const{ return _commands;}

void Server::setEpoll_fd(int epoll_fd) { this->_epoll_fd = epoll_fd; }
int Server::getEpoll_fd() const { return this->_epoll_fd; }

// struct epoll_event* Server::getEventEpoll_s()  { return &s_event_epoll;}
// struct epoll_event* Server::getEventsEpoll_m()  { return m_events_epoll;}


void Server::enableSendEvent(int epoll_fd, int fd_client)
{
    struct epoll_event epoll_v ;

    epoll_v.events = EPOLLIN | EPOLLOUT;
    epoll_v.data.fd = fd_client;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD,fd_client,&epoll_v) < 0)
        std::cerr << "Error on SendEvent" << std::endl; 
}

//eli function
std::string Server::servername(void) const 
{
	return ("MyircServer");
}
