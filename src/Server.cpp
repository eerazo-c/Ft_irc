#include "Server.hpp"

Server::Server() : _port_s(0),_password(""),_server_socket(-1)
{
    std::memset(&this->_server_address, 0, sizeof(this->_server_address));
    std::cout << "no deberias estar haciendo esto;" << std::endl;
}

Server &Server::operator=(const Server &orignal)
{
    if (this != &orignal) {}return *this;
}
//
Server::Server(const Server &to_copy)
{
    *this = to_copy;
}

Server::Server(int port, std::string &password)
{
    
    try
    {
        setPort(port);
        setPass(password);

        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0)
            throw("Error socket");
        setServer_socket(server_socket);
        if (setNonBlocking_socket(this->getServer_socket()) == -1)
            throw("Error fcntl");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
}


Server::~Server()
{
    // cerrar los fd de los clientes tambien
    close (_server_socket);
}


void Server::addClient(int fd, Client cliente)
{
    cliente.setFd(fd);
    _clients.insert(std::pair<int, Client>(fd, cliente));
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
       // close()  fds
        throw("Error in bind");
        return -1;
    }
    return 1;
}
int Server::listenServer()
{
   if (listen(getServer_socket(), MAX_CONECTIONS) < 0)
    {
        //closee   
        throw("Error Listen");
        return -1;
    }
    return 1;
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

std::map<int, Client>& Server::getClients(){ return _clients;}
// int Server::getEpoll_fd() const{ return epoll_fd; }

// struct epoll_event* Server::getEventEpoll_s()  { return &s_event_epoll;}
// struct epoll_event* Server::getEventsEpoll_m()  { return m_events_epoll;}
