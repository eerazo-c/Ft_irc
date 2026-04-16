#ifndef SERVER_H
#define SERVER_H

#pragma once

#include "header.h"

class Server
{
    private:
     /* int _port_s;
        std::string  _password;
        int _server_socket;
        struct sockaddr_in _server_address;
        
    */
    public:
        Server();
        Server(const Server &to_copy);
        Server(int &port, std::string &password);
        Server &operator=(const Server &orignal);
        ~Server();

    // para luego el tipo de exception 
    class Exe : public std::exception
    {
        virtual const char * what() const throw();
    };

};

#endif