#include "Server.h"

Server::Server()
{

}

Server::Server(const Server &to_copy)
{
    *this = to_copy;
}

Server::Server(int &port, std::string &password)
{
    (void)port;
    (void)password;
}

Server &Server::operator=(const Server &orignal)
{
    if (this != &orignal)
    {

    }
    return *this;
}


Server::~Server()
{

}