#include "header.h"
#include "Server.h"
#include "Client.hpp"


std::map<int, Client> clients;

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
        std::cout << "command: " << command << std::endl;
    }
    else{
        command = buffer;
        buffer.clear();
    }

    if ((trailing_pos = buffer.find(" :")) != std::string::npos){
        trailing_param = buffer.substr(trailing_pos + 2, std::string::npos);
        std::cout << "trailing argument: " << trailing_param << std::endl;
        buffer.erase(trailing_pos);
        int i = 0;
        while((pos = buffer.find(" ")) != std::string::npos){
            params.push_back(buffer.substr(0, pos));
            buffer.erase(0, pos + 1);
            i++;
        }
        params.push_back(buffer);
    }
    else{
        int i = 0;
        while((pos = buffer.find(" ")) != std::string::npos){
            params.push_back(buffer.substr(0, pos));
            buffer.erase(0, pos + 1);
            i++;
        }
        params.push_back(buffer);
    }
    std::for_each(params.begin(), params.end(), printElement);
}

void handleClientData(Client& client , char *tempBuffer){
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
    if (ar !=3)
    {
        std::cout << RED << "ERROR: Invalid Arguments" << RESET <<
        std::endl << YELLOW << "usage: /ircserv <port> <password>" << RESET<< std::endl;
        return 1;
    }
    std::cout << "Todo bien jeje" << std::endl;
    std::string in_port (argv[1]);
    std::string in_password (argv[2]);
    //Server main_server(in_port, in_password);

 
    int socket_t = socket(AF_INET, SOCK_STREAM, 0); // renombar a serversocket despues del test
    if (socket_t < 0)
    {
        std::cerr << "Error al crear el socket" << std::endl;
        return 1;
    }
    if (setNonBlocking(socket_t) == -1)
    {
        std::cerr << "Error al fcntl 1" << std::endl;
        return 1;
    }

    struct sockaddr_in server_address;

    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Escuchar en cualquier IP de esta PC
    server_address.sin_port = htons(std::atoi(argv[1]));//htons transforma un int a una networkbite (orden de bytes)


    int opt = 1;
    setsockopt(socket_t, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));
    // Ata el socket al puerto pasado por parametro   ej 8080
    if (bind(socket_t, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Error in bind;" << std::endl;
        return 1;
    }
    
    // Ponerse a escuchar (máximo 3 personas en fila)
    if (listen(socket_t, MAX_CONECTIONS) < 0)
    {
        std::cerr << "Error in listen;" << std::endl;
        return 1;
    }
    std::cout << "Esperando cliente..." << std::endl;
    std::cout << "Servidor escuchando en el puerto "<< in_port << std::endl;
    
    // prueba del epoll()
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        std::cerr << "Error in epoll;" << std::endl;
        close(socket_t);
        return 1;
    }
    
    struct epoll_event event_epoll , events_epoll[10];
    event_epoll.events = EPOLLIN | EPOLLET; // Avisar cuando haya datos de entrada (nuevas conexiones)
    event_epoll.data.fd = socket_t;   // Asociar nuestro socket principal
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_t, &event_epoll)< 0)
    {
        std::cerr << "Error in epoll_ctl" << std::endl;
        return 1;
    }

    while (true)
    { 
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

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
            if (events_epoll[i].data.fd == socket_t)
            {
                // A) Si el evento es en el socket principal: ES UN CLIENTE NUEVO
                int new_socket = accept(socket_t, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
                if(new_socket < 0)
                { 
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                        break;
                    std::cerr << "Error in accept" << std::endl;
                    break;
                }
                if (setNonBlocking(new_socket) == -1)
                {
                    std::cerr << "Error al fcntl 2" << std::endl;
                    return 1;
                }
                
                Client clien (new_socket);
                clients.insert(std::pair<int, Client>(clien.getFd(), clien));
                
                // aqui agregar validacion de cliente
                /*char buffer[1024] = {0};
                if (recv(clien.getFd(), buffer, sizeof(buffer) -1 , 0) <= 0)
                {
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clien.getFd(), NULL);
                    std::cout << YELLOW << "Cliente desconectado.  on 0" << RESET << std::endl;
                    close(clien.getFd());
                        continue;
                }
                handleClientData(clien, buffer);*/



                // Añadimos el NUEVO cliente a la vigilancia de epoll
                struct epoll_event new_event_c;
                new_event_c.events = EPOLLIN; 
                new_event_c.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &new_event_c);
                std::cout << "Nuevo cliente conectado y vigilado." << std::endl;

            } 
            else 
            {
                // B) Si el evento es en otro socket: ES UN CLIENTE MANDANDO DATOS
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
                    if (send(client_fd, "OK", 2, 0) == -1)
                    {
                        std::cerr << "Error in send" << std::endl;
                       continue;
                    }
                    
                    handleClientData(clients[epoll_fd], buffer);
                }
            }
       
        }
    }
    close (epoll_fd);
    close(socket_t);
    return 0;
}