#pragma once

#include "header.hpp"
#include "Server.hpp"
class Server;

class Client{
    public:
        enum AuthState{
            UNREGISTERED,
            PASS_ACCEPTED,
            REGISTERED,
        };

    private:
        int _fd;
        std::string _nick;
        std::string _user;
        std::string _message;
        AuthState _state;
        struct sockaddr_in _client_addr;
        socklen_t _addrlen;
	//	bool _closing;

    public:
        Client();
        Client(int fd);
        Client(const Client &other);
        Client &operator=(const Client &other);
        ~Client();

        int getFd() const;
        const std::string& getNick() const;
        const std::string& getName() const;
        std::string& getMessage();
        const AuthState& getState() const;
        const struct sockaddr_in& getClient_addres() const;
        socklen_t& getAddressLen() ;


        void setFd(int fd);
        void setNick(const std::string& nick);
        void setUser(const std::string& user);
        void setMesagge(const std::string& message);
        void setState(AuthState state);
        void setClient_addres();
		//void set_closing(bool closing); //eli funtion

		void WritePrefix(std::string const &_message); //eli funtion
		void CloseClient(Server &server); //elifuntion
};
