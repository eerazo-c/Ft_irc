#include "header.hpp"

int main(int ar, char const *argv[])
{
    if(check_args(ar, argv[1], argv[2]) == -1)
        return 1;   
    
    std::string in_password(argv[2]);
    Server irccserver(std::atoi(argv[1]), in_password, "Rocket");

    char hostaname[256];
    if (gethostname(hostaname, sizeof(hostaname)) == -1) {
        std::cerr << "hostname error" << std::endl;
        return 1;
    }

    struct hostent* host_e = gethostbyname(hostaname);
    if (host_e == NULL) {
        herror("gethostbyname error");
        return 1;
    }

    char *ip_loc = inet_ntoa(*((struct in_addr*)host_e->h_addr_list[0]));

    try {    
        irccserver.setIP(ip_loc);
        irccserver.start();
        irccserver.run();
    } catch(const std::exception& e) {
        std::cerr << MAGENTA << e.what() << RESET << std::endl;
        return 1;
    }
    
    return 0;
}
