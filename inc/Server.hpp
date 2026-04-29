#ifndef SERVER_HPP
#define SERVER_HPP

#pragma once

#include "header.hpp"
#include "Parser.hpp"

class Command;
class Parser;
struct IrcMessage;

class Server 
{
    private:
        int _port_s;
        std::string  _password;
        int _server_socket;
        struct sockaddr_in _server_address;

        std::map<int ,Client> _clients;
        std::map<std::string, Command*> _commands;
        Server();       
        Server(const Server &to_copy);
        Server &operator=(const Server &orignal);

    public:
        
        Server(int port, std::string &password);
        ~Server();

        int setNonBlocking_socket(int socket_s);
        
        void setSockectReusable();
        
        int bindSocketToServer();
        int listenServer();

        void handleClientData(Client& client , const std::string& tempBuffer, Parser& parser);
        void executeCommand(Client& client, IrcMessage& message);

        void setPort(int port);
        void setPass(std::string pass);
        void setServer_socket(int socket);
        void setServer_address();
       

        void addClient(int fd, Client client);
        std::map<int ,Client>& getClients();
        const std::map<std::string, Command*>& getCommands() const;
        std::string getPass() const ;
        int getPort() const;
        int getServer_socket() const;
        struct sockaddr_in& getServer_address();
        // para luego el tipo de exception 
    class Error_fd : public std::exception
    {
        virtual const char * what() const throw();
    };
};

#endif