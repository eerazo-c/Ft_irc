#include "header.h"

int main(int ar , char**argv)
{
	if (ar !=3)
    {
        std::cout << "ERROR: Invalid Arguments" <<
        std::endl << "usage: /ircserv <port> <password>" << std::endl;
        return 0;
    }
    std::cout << "Todo bien jeje" << std::endl;
    std::string in_port (argv[1]);
    std::string in_password (argv[2]);
	//aqui empieza el cliente
	
 //AF_INET = IPv4  , AF_INET6 = IPv6
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    int socket_t = socket(AF_INET, SOCK_STREAM , 0);
    if (socket_t < 0)
    {
        std::cerr << "Error al crear el socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_data;
    std::memset(&server_data, 0, sizeof(server_data));
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(std::atoi(argv[1])); //htons transforma un int a una networkbite (orden de bytes)
    if (inet_pton(AF_INET, "127.0.0.1", &server_data.sin_addr) <= 0) //inet_pton convierte  char* de la ip a una direcion de red 
    {
        std::cerr << "Dirección IP inválida" << std::endl;
        return 1;
    }
    // sockaddr vs sockaddr_in
    // sockaddr es una struct generica  que esperan recibir un protocolo ipv4, ipv6 , etc.
    // sockaddr_in  struct para ipv4 donde podemos añadir ip y puerto
    //  hacemos cast de sockaddr_in a sockaddr porque lo pide la funcion connect()
    if (connect(socket_t, (struct sockaddr *)&server_data, sizeof(server_data)) < 0)
     {
        std::cerr << "Conexión fallida" << std::endl;
        close(socket_t);
        return 1;
    }
    std::cout << "¡Conectado exitosamente!" << std::endl;

		// 1. Definir los comandos (IRC requiere que terminen en \r\n)
		std::string pass_cmd = "PASS " + in_password + "\r\n";
		std::string nick_cmd = "NICK mi_bot\r\n";
		std::string user_cmd = "USER guest 0 * :Soy un cliente\r\n";
		std::string msg_cmd  = "PRIVMSG #canal_test :Hola desde mi cliente C++\r\n";

		// 2. Enviar los datos con send()
		// ssize_t send(int sockfd, const void *buf, size_t len, int flags);

		send_irc_cmd(socket_t, pass_cmd);
		send_irc_cmd(socket_t, nick_cmd);
		send_irc_cmd(socket_t, user_cmd);
	
		// Esperar un poco o entrar en un bucle de lectura antes de enviar el mensaje
		// para asegurar que el servidor nos ha aceptado.
		sleep(1); 

		send(socket_t, msg_cmd.c_str(), msg_cmd.length(), 0);

		std::cout << "Mensajes enviados." << std::endl;


    close(socket_t);
	
	return 0;
} 

void send_irc_cmd(int sock, std::string cmd) {
    cmd += "\r\n";
    if (send(sock, cmd.c_str(), cmd.length(), 0) < 0) {
        std::cerr << "Error al enviar comando" << std::endl;
    }
}  
