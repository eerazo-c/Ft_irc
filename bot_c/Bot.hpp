#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


class Bot
{



    private:
        enum AuthState{};
        int _fd;
        std::string _botNick;
        std::string _botUser;
        std::string _botUser;
        std::string _message;
        int _state;
        AuthState _state;

        struct sockaddr_in _bot_addr;
        socklen_t _addrlen;

    public:
        enum AuthState
        {
            UNREGISTERED,
            PASS_ACCEPTED,
            REGISTERED,
        };

        Bot();
        Bot(std::string nick )

        Bot(const Bot &to_copy);
        Bot &operator=(const Bot &original);
        ~Bot();    

        const std::string &getNick() const ;
        const std::string &getBotUser() const ;
        const std::string &getBotUser() const ;
        const std::string &getMessage() const ;
        const AuthState& getState();
        int getFd();
        const struct sockaddr_in &getClient_addres() const ;
        socklen_t& getAddreslen();

        void setFd(int fd);
        void setMessage(std::string &msg);
        void setState(AuthState state);
        void setClient_addres();

};
