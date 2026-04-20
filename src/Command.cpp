/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:09:18 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/20 14:20:02 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "Command.hpp"

Command::Command(){

}

Command::Command(const Command &obj){
	*this = obj;
}

Command &Command::operator=(const Command &obj)
{
	if (this != &obj)
		this->typeCmd = obj.getCommand();
	return (*this);
}

Command::~Command(){

}

void Command::execute(void) const{
	std::cout << "aqui pasarian cosas, no se que" << std::endl;
}

std::string Command::getCommand(void) const{
	return (this->typeCmd);
}

Command::Cp(void)
{
	typeCmd["PASS"];
	/*commands_["INVITE"] = new Invite;
	commands_["JOIN"] = new Join;
	commands_["KICK"] = new Kick;
	commands_["MODE"] = new Mode;
	commands_["NICK"] = new Nick;
	commands_["PART"] = new Part;
	commands_["PRIVMSG"] = new PrivMsg;
	commands_["QUIT"] = new Quit;
	commands_["TOPIC"] = new Topic;
	commands_["USER"] = new User;*/
}
