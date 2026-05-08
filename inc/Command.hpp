/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhea <arhea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:00:40 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/07 18:06:59 by arhea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "header.hpp"
#include <sstream>
#include <cmath>

class Command 
{
	public:
		virtual ~Command(){}
		virtual void execute(Client& client, std::vector<std::string> args, Server &server) const = 0;

};

class Pass : public Command
{
	public:
		~Pass();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Nick : public Command
{
	public:
		~Nick();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class User : public Command
{
	public:
		~User();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Join : public Command
{
	public:
		~Join();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Part : public Command
{
	public:
		~Part();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Quit : public Command
{
	public:
		~Quit();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class PrivMsg : public Command
{
	public:
		~PrivMsg();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Kick : public Command
{
	public:
		~Kick();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Invite : public Command
{
	public:
		~Invite();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Topic : public Command
{
	public:
		~Topic();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

class Mode : public Command
{
	public:
		~Mode();
		void execute(Client& client, std::vector<std::string> args, Server &server) const;
};

#endif
