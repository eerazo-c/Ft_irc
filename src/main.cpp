#include "../inc/header.h"
#include "../inc/Server.h"



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


    //TODO 
    // crear socket
    // linkar el socket a un puerto 
    // hacer que socket este escuchando
    // aceptar la llamada
    // cerrar la llamada del socket
    // mientras reciven la señal   mostrar mensaje 
    // cerrar el socket

    // prueba abriendo un socket  cliente
    //AF_INET = IPv4  , AF_INET6 = IPv6
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    int socket_t = socket(AF_INET, SOCK_STREAM, 0); // renombar a serversocket despues del test
    if (socket_t < 0)
    {
        std::cerr << "Error al crear el socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Escuchar en cualquier IP de esta PC
    server_address.sin_port = htons(std::atoi(argv[1]));//htons transforma un int a una networkbite (orden de bytes)

    // sockaddr vs sockaddr_in
    // sockaddr es una struct generica  que esperan recibir un protocolo ipv4, ipv6 , etc.
    // sockaddr_in  struct para ipv4 donde podemos añadir ip y puerto
    //  hacemos cast de sockaddr_in a sockaddr porque lo pide la funcion concreta

    // Ata el socket al puerto pasado por parametro   ej 8080
    if (bind(socket_t, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Error in bind;" << std::endl;
        return 1;
    }
    
    // Ponerse a escuchar (máximo 3 personas en fila)
    if (listen(socket_t, 3) < 0)
    {
        std::cerr << "Error in listen;" << std::endl;
        return 1;
    }
    std::cout << "Esperando cliente..." << std::endl;
    std::cout << "Servidor escuchando en el puerto "<< in_port << std::endl;
    

    while (true)
    {
        struct sockaddr_in client_address;
        socklen_t client_leng = sizeof(client_address);   
        
        std::cout << "Waiting for the conextion" << std::endl;
        int new_client_socket = accept(socket_t,(struct sockaddr*) &client_address, &client_leng);
        if (new_client_socket < 0)
        {
            std::cerr << "Error al aceptar el cliente" << std::endl;
            continue;
        }
        std::cout << "Cliente conectado desde: " << inet_ntoa(client_address.sin_addr) << std::endl;
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        if (recv(new_client_socket, buffer, 1024, 0) > 0)
        {
            std::cout << "Mensage recivido: " << buffer <<std::endl;

            const char* respuesta = "Mensaje recibido correctamente";
            send(new_client_socket, respuesta, std::strlen(respuesta), 0);
        }
        close(new_client_socket);
         std::cout << "Conexion con cliente cerrada. Volviendo a escuchar..." << std::endl;
    }
    
    

    // char buffer[1024] = {0};
    // if (recv(new_socket, buffer, 1024, 0))
    // {
    //     std::cerr << "Error in recv" << std::endl;
    //     return 1;
    // }
    // std::cout << "Mensaje recibido: " << buffer << std::endl;

 
    close(socket_t);

    return 0;
}


/*
int main() {
    // 1. Crear socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error al crear socket" << std::endl;
        return 1;
    }

    // 2. Configurar dirección (Escuchar en puerto 8080)
    struct sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones de cualquier IP
    address.sin_port = htons(8080);

    // 3. Atar (Bind) el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error en bind (¿puerto ocupado?)" << std::endl;
        return 1;
    }

    // 4. Escuchar (Listen)
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Error en listen" << std::endl;
        return 1;
    }

    std::cout << "Servidor escuchando en el puerto 8080..." << std::endl;

    // 5. Bucle infinito para aceptar clientes
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);

        std::cout << "\nEsperando una nueva conexion..." << std::endl;

        // El programa se detiene aquí hasta que llega un cliente
        int new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
        
        if (new_socket < 0) {
            std::cerr << "Error al aceptar cliente" << std::endl;
            continue; // Ir a la siguiente iteración
        }

        std::cout << "Cliente conectado desde: " << inet_ntoa(client_addr.sin_addr) << std::endl;

        // Leer datos del cliente
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        int valread = recv(new_socket, buffer, 1024, 0);

        if (valread > 0) {
            std::cout << "Mensaje recibido: " << buffer << std::endl;
            
            // Responder al cliente
            const char* respuesta = "Mensaje recibido correctamente";
            send(new_socket, respuesta, std::strlen(respuesta), 0);
        }

        // Importante: Cerrar solo el socket del cliente, NO el del servidor
        close(new_socket);
        std::cout << "Conexion con cliente cerrada. Volviendo a escuchar..." << std::endl;
    }

    // Nunca llegará aquí, pero por buena práctica:
    close(server_fd);
    return 0;
}
*/


/* con epoll() despues del paso 4 

// 5. Crear la instancia de epoll
int epoll_fd = epoll_create(1); 
struct epoll_event event, events[10]; // "event" para configurar, "events" para recibir

// 6. Añadir el socket del servidor a la vigilancia
event.events = EPOLLIN;      // Avisar cuando haya datos de entrada (nuevas conexiones)
event.data.fd = server_fd;   // Asociar nuestro socket principal
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

while (true) {
    // 7. Esperar a que ocurra algo (se bloquea aquí de forma eficiente)
    int num_eventos = epoll_wait(epoll_fd, events, 10, -1);

    for (int i = 0; i < num_eventos; i++) {
        if (events[i].data.fd == server_fd) {
            // A) Si el evento es en el socket principal: ES UN CLIENTE NUEVO
            int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            
            // Añadimos el NUEVO cliente a la vigilancia de epoll
            event.events = EPOLLIN; 
            event.data.fd = new_socket;
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event);
            std::cout << "Nuevo cliente conectado y vigilado." << std::endl;

        } else {
            // B) Si el evento es en otro socket: ES UN CLIENTE MANDANDO DATOS
            int client_fd = events[i].data.fd;
            char buffer[1024] = {0};
            int bytes = recv(client_fd, buffer, 1024, 0);

            if (bytes <= 0) {
                // Cliente desconectado
                close(client_fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                std::cout << "Cliente desconectado." << std::endl;
            } else {
                std::cout << "Recibido: " << buffer << std::endl;
                send(client_fd, "OK", 2, 0);
            }
        }
    }
}

*/