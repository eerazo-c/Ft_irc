#pragma once

#include "header.hpp"

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
        std::string _realName;
        std::string _message;
        AuthState _state;

        struct sockaddr_in _client_addr;
        socklen_t _addrlen;

    public:
        Client();
        Client(int fd);
        Client(const Client &other);
        Client &operator=(const Client &other);
        ~Client();

        int getFd() const;
        const std::string& getNick() const;
        const std::string& getUser() const;
        const std::string& getRealName() const;
        std::string& getMessage();
        const AuthState& getState() const;
        const struct sockaddr_in& getClient_addres() const;
        socklen_t& getAddressLen();


        void setFd(int fd);
        void setNick(const std::string& nick);
        void setUser(const std::string& user);
        void setRealName(const std::string& realName);
        void setMesagge(const std::string& message);
        void setState(AuthState state);
        void setClient_addres();
};
