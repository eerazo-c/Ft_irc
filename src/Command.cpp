/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:48:43 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/22 17:48:58 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Command.hpp"
#include "Channel.hpp"

Pass::~Pass(){}
void Pass::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Pass execute" << std::endl;
}

Nick::~Nick(){}
void Nick::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Nick execute" << std::endl;
}

User::~User(){}
void User::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "User execute" << std::endl;
}

Join::~Join() : command("JOIN")
{

}

void Join::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
	if (args.size() == 1 && args[0] == "0")
	{
		server.getChannel().partAll
	}
    std::cout << "Join execute" << std::endl;
}

Part::~Part(){}
void Part::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Part execute" << std::endl;
}

Quit::~Quit(){}
void Quit::execute(Client& client, std::vector<std::string> args, Server &server) const{
    (void)client;
    (void)args;
    (void)server;
    std::cout << "Quit execute" << std::endl;
}
