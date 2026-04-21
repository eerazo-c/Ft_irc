#include "header.h"
#include "Server.h"

void printElement(std::string str){
    std::cout << "argument: " << str << std::endl;
}

void parser(std::string buffer){
    std::string command;
    std::vector<std::string> params;
    std::string trailing_param;
    size_t pos = std::string::npos;
    size_t trailing_pos = std::string::npos;

    if ((pos = buffer.find(" ")) != std::string::npos){
        command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
    }
    else{
        command = buffer;
        buffer.clear();
    }
    std::cout << "command: " << command << std::endl;

    if ((trailing_pos = buffer.find(" :")) != std::string::npos){
        trailing_param = buffer.substr(trailing_pos + 2);
        buffer.erase(trailing_pos);
        if (!trailing_param.empty()){
            std::cout << "trailing argument: " << trailing_param << std::endl;
        }
    }

    while((pos = buffer.find(' ')) != std::string::npos){
        std::string token = buffer.substr(0, pos);
        if (!token.empty())
            params.push_back(token);
        buffer.erase(0, pos + 1);
    }
    params.push_back(buffer);

    std::for_each(params.begin(), params.end(), printElement);
}

void handleClientData(Client& client , std::string tempBuffer){
    client.setMesagge(client.getMessage() + tempBuffer);

    std::size_t pos = 0;
    std::string currentBuffer = client.getMessage();

    while((pos = currentBuffer.find("\r\n")) != std::string::npos){
        std::string command = currentBuffer.substr(0, pos);
        currentBuffer.erase(0, pos + 2);
        std::cout << "message: " << command << "$" << std::endl;
        parser(command);
    }
    client.setMesagge(currentBuffer);
}

int main(int ar, char const *argv[])
{
    
    if(check_args(ar , argv[1], argv[2]) == -1)
        return 1;   
    
    std::string in_password (argv[2]);
    Server irccserver (std::atoi(argv[1]), in_password);
    std::memset(&(irccserver.getServer_address()), 0, sizeof(irccserver.getServer_address()));
    try
    {    
        irccserver.setServer_address();
        irccserver.setSockectReusable();    
        irccserver.bindSocketToServer();
        irccserver.listenServer();
        ft_message("Escuchando ...", NULL,1);    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    //////////////////////////
    // prueba del epoll()
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        std::cerr << "Error in epoll;" << std::endl;
        close(irccserver.getServer_socket());
        return 1;
    }
    
    struct epoll_event event_epoll , events_epoll[10];
    event_epoll.events = EPOLLIN | EPOLLET; // Avisar cuando haya datos de entrada (nuevas conexiones)
    event_epoll.data.fd = irccserver.getServer_socket();   // Asociar nuestro socket principal
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, irccserver.getServer_socket(), &event_epoll)< 0)
    {
        std::cerr << "Error in epoll_ctl" << std::endl;
        return 1;
    }

    while (true)
    { 
        int num_eventos = epoll_wait(epoll_fd, events_epoll, 10, -1);
        if(num_eventos < 0)
        {
            if (errno == EINTR)
            {
                std::cerr << "Error in epollwait;" << std::endl;
                continue;
            }           
        }
        for (int i = 0; i < num_eventos; i++)
        {
            if (events_epoll[i].data.fd == irccserver.getServer_socket())
            {
                Client nclient;
                nclient.getAddressLen() = sizeof (struct sockaddr_in);

                int new_socket = accept(irccserver.getServer_socket(),
                 (struct sockaddr *)&nclient.getClient_addres(),
                  &nclient.getAddressLen());
                if(new_socket < 0)
                { 
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    std::cerr << "Error in accept" << std::endl;
                    break;
                }
                if (irccserver.setNonBlocking_socket(new_socket) == -1)
                {
                    std::cerr << "Error al fcntl 2" << std::endl;
                    break;
                }
                
                irccserver.addClient(new_socket,nclient);

                struct epoll_event new_event_c;
                new_event_c.events = EPOLLIN; 
                new_event_c.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &new_event_c);
                std::cout << "Nuevo cliente conectado y vigilado." << std::endl;

            } 
            else 
            {
                int client_fd = events_epoll[i].data.fd;
                char buffer[1024] = {0};
                int bytes = recv(client_fd, buffer, sizeof(buffer) -1 , 0);

                if (bytes == -1)
                {
                    if(errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                        std::cout << YELLOW << "Cliente desconectado. on -1" << RESET << std::endl;
                        close(client_fd);
                    }
                    continue;
                } 
                else if (bytes == 0)
                {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    std::cout << YELLOW << "Cliente desconectado.  on 0" << RESET << std::endl;
                    close(client_fd);
                    continue;
                }
                else 
                {
                    
                    std::cout << GREEN << "Recibido: " << buffer << RESET << std::endl;
                    if (irccserver.sendhandshake(client_fd) == -1)
                    {
                        std::cerr << "Error in handshake" << std::endl;
                       continue;
                    }
                    handleClientData(irccserver.getClients()[client_fd], std::string(buffer));
                }
            }
       
        }
    }
    close (epoll_fd);
    close(irccserver.getServer_socket());
    return 0;
}