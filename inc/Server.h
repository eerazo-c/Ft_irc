#ifndef SERVER_H
#define SERVER_H

#pragma once

#include "header.h"

class Server
{
    private:

    public:
        Server();
        Server(const Server &to_copy);
        Server(std::string &port, std::string &password);
        Server &operator=(const Server &orignal);
        ~Server();

    // para luego el tipo de exception 
    class Exe : public std::exception
    {
        virtual const char * what() const throw();
    };

};

#endif