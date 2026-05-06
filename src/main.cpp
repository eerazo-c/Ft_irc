#include "header.hpp"

//std::map<int, Client> clients;

int  setNonBlocking(int fd)
{
    int flags; 
    while ((flags = fcntl(fd, F_GETFL, 0)) == -1)
    {
        if (errno != EINTR)
            return -1;
    }
    while (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        if (errno != EINTR)  
            return -1;
    }
    return 1;
}

int main(int ar, char const *argv[])
{
    
    if(check_args(ar , argv[1], argv[2]) == -1)
        return 1;   
    
    std::string in_password (argv[2]);
    Server irccserver (std::atoi(argv[1]), in_password, "INEA_ecuatorial");
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

    Parser parser;

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
				//aqui la modificacion
                Client *nclient = new Client();
//				irccserver.addClient(new_socket, nclient);
                nclient->getAddressLen() = sizeof (struct sockaddr_in);

                int new_socket = accept(irccserver.getServer_socket(),
                 (struct sockaddr *)&nclient->getClient_addres(),
                  &nclient->getAddressLen());
                if(new_socket < 0)
                { 
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    std::cerr << "Error in accept" << std::endl;
                    break;
                }

				nclient->setFd(new_socket);  //FALTABA

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
					std::map<int, Client*>& clients = irccserver.getClients();
					std::map<int, Client*>::iterator it = clients.find(client_fd);

                    /*Client* cl = irccserver.getClients()[client_fd];

					if (!cl)
					{
    				std::cout << "CLIENTE NULL 💀" << std::endl;
    				continue;
					}*/

					irccserver.handleClientData(*(it->second), std::string(buffer), parser);
                //    irccserver.handleClientData(*irccserver.getClients()[client_fd], std::string(buffer), parser);
                }
            }
       
        }
    }
    close (epoll_fd);
    close(irccserver.getServer_socket());
    return 0;
}
