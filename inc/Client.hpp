#pragma once

#include "header.h"

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

        void setFd(int fd);
        void setNick(const std::string& nick);
        void setUser(const std::string& user);
        void setMesagge(const std::string& message);
        void setState(AuthState state);
};
