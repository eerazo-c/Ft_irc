#include "../inc/header.h"
#include "../inc/Server.h"



int main(int ar, char const *argv[])
{
    if (ar !=3)
    {
        std::cout << RED << "ERROR: Invalid Arguments" << RESET <<
        std::endl << YELLOW << "usage: /ircserv <port> <password>" << RESET<< std::endl;
        return 0;
    }
    std::cout << "Todo bien jeje" << std::endl;
    std::string in_port (argv[1]);
    std::string in_password (argv[2]);
    Server main_server(in_port, in_password);
    

    return 0;
}
