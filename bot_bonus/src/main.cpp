/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 19:47:35 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/11 19:47:58 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "header_client.hpp"

void send_irc_cmd(int sock, std::string cmd) 
{
    cmd += "\r\n";
    if (send(sock, cmd.c_str(), cmd.length(), 0) < 0) {
        std::cerr << "Error en SEND" << std::endl;
    }
}

std::string get_nick(std::string buffer) {
    size_t bang = buffer.find('!');
    if (bang != std::string::npos && buffer[0] == ':') {
        return buffer.substr(1, bang - 1);
    }
    return "";
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cout << "Usage: ./bot_irc <port> <password>" << std::endl;
        return 1;
    }

    std::string in_password(argv[2]);
    srand(time(NULL));

    int socket_t = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_t < 0) return 1;

    struct sockaddr_in server_data;
    std::memset(&server_data, 0, sizeof(server_data));
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(std::atoi(argv[1]));
    inet_pton(AF_INET, "127.0.0.1", &server_data.sin_addr);

    if (connect(socket_t, (struct sockaddr *)&server_data, sizeof(server_data)) < 0) {
        std::cerr << "Error: connect" << std::endl;
        return 1;
    }

    fcntl(socket_t, F_SETFL, O_NONBLOCK);

    std::cout << "Conectado :D" << std::endl;

    send_irc_cmd(socket_t, "PASS " + in_password);
    send_irc_cmd(socket_t, "NICK HALLL");
    send_irc_cmd(socket_t, "USER bot 0 * :Robot42");
    send_irc_cmd(socket_t, "JOIN #general");

    std::vector<std::string> randMsgm;
    randMsgm.push_back("¡Hola, soy un bot de 42!");
    randMsgm.push_back("¿Alguien ha visto a mi programador?");
    randMsgm.push_back("¡C++ es mejor que el lenguaje ensamblador!");
    randMsgm.push_back("¿Sabías que el IRC es de 1988?");
    randMsgm.push_back("¡42 Barcelona manda!");

    time_t lastMsg = time(NULL);
    char buffer[1024];

    while (true)
    {
        std::memset(buffer, 0, 1024);
        int bytes = recv(socket_t, buffer, 1023, 0);
        if (bytes > 0) {
            std::string msg(buffer);
            std::cout << "Servidor dice: " << msg;

            if (msg.find("PING") == 0) {
                send_irc_cmd(socket_t, "PONG " + msg.substr(5));
            }

            if (msg.find(" JOIN ") != std::string::npos) {
                std::string new_nick = get_nick(msg);
                if (!new_nick.empty() && new_nick != "HALLL") {
                    send_irc_cmd(socket_t, "PRIVMSG #general :¡Hola " + new_nick + "! Bienvenido al canal :D");
                }
            }
        }

        if (time(NULL) - lastMsg >= 10)
        {
            int r = rand() % randMsgm.size();
            send_irc_cmd(socket_t, "PRIVMSG #general :" + randMsgm[r]);
            lastMsg = time(NULL);
        }

        usleep(100000);
    }

    close(socket_t);
    return 0;
}
