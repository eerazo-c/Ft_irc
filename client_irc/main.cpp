#include "header_client.hpp"


void send_irc_cmd(int sock, const std::string &cmd)
{
    std::string full_cmd = cmd + "\r\n";
    if (send(sock, full_cmd.c_str(), full_cmd.length(), 0) < 0)
        std::cerr << "Error al enviar comando" << std::endl;
    else
        std::cout << ">> " << cmd << std::endl;
}

void handle_server_msg(int sock, std::string buffer)
{
    std::cout << "<< " << buffer;

    // Responder a PING automáticamente
    if (buffer.find("PING") == 0)
    {
        std::string token = buffer.substr(5); // después de "PING "
        send_irc_cmd(sock, "PONG " + token);
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Uso: ./irc_client <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(av[1]);
    std::string password = av[2];

    // Crear socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Error al crear socket" << std::endl;
        return 1;
    }

    // Configurar dirección
    struct sockaddr_in server;
    std::memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0)
    {
        std::cerr << "IP inválida" << std::endl;
        return 1;
    }

    // Conectar
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::cerr << "Error al conectar" << std::endl;
        return 1;
    }

    std::cout << "Conectado al servidor IRC" << std::endl;

    // Registro IRC correcto
    send_irc_cmd(sock, "PASS " + password);
    send_irc_cmd(sock, "NICK mi_bot");
    send_irc_cmd(sock, "USER guest 0 * :Soy un cliente");

    sleep(1);

    // Unirse a canal
    //send_irc_cmd(sock, "JOIN #canal_test");

    sleep(1);

    // Enviar mensaje inicial
    //send_irc_cmd(sock, "PRIVMSG #canal_test :Hola desde cliente tester");

    // Loop principal (leer servidor + stdin)
    fd_set fds;
    char buffer[512];

    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(0, &fds); // stdin

        int maxfd = sock;

        if (select(maxfd + 1, &fds, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Error en select" << std::endl;
            break;
        }

        // Mensaje del servidor
        if (FD_ISSET(sock, &fds))
        {
            int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0)
            {
                std::cout << "Desconectado del servidor" << std::endl;
                break;
            }
            buffer[bytes] = '\0';
            handle_server_msg(sock, buffer);
        }

        // Input del usuario
        if (FD_ISSET(0, &fds))
        {
            std::string input;
            if (!std::getline(std::cin, input))
                break;

            if (!input.empty())
                send_irc_cmd(sock, input);
        }
    }

    close(sock);
    return 0;
}