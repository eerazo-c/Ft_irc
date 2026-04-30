#include "Client.hpp"

Client::Client() : _fd(-1), _nick(""), _user(""), _message(""), _state(UNREGISTERED){}

Client::Client(int fd) : _fd(fd), _nick(""), _user(""), _message(""), _state(UNREGISTERED){}

Client::Client(const Client &other) : _fd(other._fd), _nick(other._nick), _user(other._user), _message(other._message), _state(other._state){}

Client &Client::operator=(const Client &other){
    if (this != &other){
        _fd = other._fd;
        _nick = other._nick;
        _user = other._user;
        _message = other._message;
        _state = other._state;
    }
    return *this;
}

Client::~Client(){}

int Client::getFd() const{return _fd;}
const std::string& Client::getNick() const{return _nick;}
const std::string& Client::getName() const{return _user;}
std::string& Client::getMessage(){return _message;}
const Client::AuthState& Client::getState() const{return _state;}
const struct sockaddr_in& Client::getClient_addres() const{ return this->_client_addr;}
socklen_t& Client::getAddressLen()  {return this->_addrlen;}



void Client::setFd(int fd){_fd = fd;}
void Client::setNick(const std::string& nick){_nick = nick;}
void Client::setUser(const std::string& user){_user = user;}
void Client::setMesagge(const std::string& message){_message = message;}
void Client::setState(AuthState state){_state = state;}

void Client::setClient_addres()
{
    std::memset(&this->_client_addr, 0, sizeof(this->_client_addr));   
    this->_addrlen = sizeof(_client_addr);
}

//eli funtion
void Client::WritePrefix(std::string const _message)
{
	Write(Server::irccserver->servername(), _message);
}
