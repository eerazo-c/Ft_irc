#include "header.hpp"

void wrong_args()
{
    ft_message("ERROR: Invalid Arguments", RED, 1);
    ft_message( "usage: /ircserv <port> <password>", YELLOW, 1);
}

void ft_message(std::string s, const char *color , int end_line)
{
    if (color == NULL)
    {
        if (end_line)
            std::cout << s << std::endl;
        else 
            std::cout << s;    
    }
    else 
    {
        if (end_line)
            std::cout << color << s << RESET << std::endl;
        else 
           std::cout << color << s << RESET;
    }
}

int check_args(int n, const char *port, const char *password)
{
    if (n != 3)
    {
        wrong_args();
        return (-1);
    }
    else 
    {
        std::string s_port (port);
        std::string s_password (password);
        for (size_t i = 0; i < s_port.size(); i++)
        {
            if(!std::isdigit(port[i]))
            {
                ft_message("Error Port non-digit", RED, 1);
                return (-1);
            }
        }
        if(s_password.empty())
        {
            ft_message("Error empty password", RED, 1);
            return -1;
        }
        for (size_t i = 0; i < s_password.size(); i++)
        {
            if(!std::isprint(password[i]))
            {
                ft_message("Error Password non-valid characters", RED, 1);
                return (-1);
            }
        }
        return 1;
    }
}