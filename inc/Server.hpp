#ifndef SERVER_HPP
#define SERVER_HPP

#include "header.hpp"
#include "Channel.hpp"

class Server 
{
    private:
        int _port_s;
        std::string  _password;
        int _server_socket;
        struct sockaddr_in _server_address;

        std::map<int ,Client> _clients;
        Server();

		//canales eli add
		std::map<std::string, Channel> _channels;

    public:
        
        Server(const Server &to_copy);
        Server &operator=(const Server &orignal);
    public:
        
        Server(int port, std::string &password);
        ~Server();

        int setNonBlocking_socket(int socket_s);
        
        void setSockectReusable();
        
        int bindSocketToServer();
        int listenServer();


        void setPort(int port);
        void setPass(std::string pass);
        void setServer_socket(int socket);
        void setServer_address();
       

        void addClient(int fd, Client client);
        std::map<int ,Client>& getClients();
        std::string getPass() const ;
        int getPort() const;
        int getServer_socket() const;
        struct sockaddr_in& getServer_address();
        // para luego el tipo de exception 

		//Canales eli add
		std::map<std::string, Channel>& getChannels(){
			return (_channels);
		};

		std::string servername(void) const {
			return (this->servername_);
		}

    class Error_fd : public std::exception
    {
        virtual const char * what() const throw();
    };
};

#endif
