/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 18:07:30 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/02 18:07:51 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Parser.hpp"

class Channel;
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
        std::string _serverName;

        std::map<int ,Client *> _clients;

		//canales eli add
		std::map<std::string, Channel> _channels;        

        std::map<std::string, Command*> _commands;
        Server();       
        Server(const Server &to_copy);
        Server &operator=(const Server &orignal);

    public:
        
        Server(int port, std::string &password, const char *serverN);
        ~Server();

        int setNonBlocking_socket(int socket_s);
        
        void setSockectReusable();
        
        int bindSocketToServer();
        int listenServer();
        int sendhandshake(int client_fd);

        void handleClientData(Client& client , const std::string& tempBuffer, Parser& parser);
        void executeCommand(Client& client, IrcMessage& message);

        void setPort(int port);
        void setPass(std::string pass);
        void setServer_socket(int socket);
        void setServer_address();
        void setServerName(const char * sname);

        void addClient(int fd, Client *client);
        std::map<int ,Client *>& getClients();
        const std::map<std::string, Command*>& getCommands() const;
        std::string getPass() const ;
        int getPort() const;
        int getServer_socket() const;
        std::string getServerName();
        struct sockaddr_in& getServer_address();
        // para luego el tipo de exception 

		//Canales eli add
		std::map<std::string, Channel>& getChannels(){
			return (_channels);
		};

		std::string servername(void) const; //eli add

    class Error_fd : public std::exception
    {
        virtual const char * what() const throw();
    };
};

#endif
