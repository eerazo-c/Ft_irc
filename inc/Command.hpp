/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalesso <nalesso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:00:40 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/21 22:15:04 by nalesso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once
#include "Channel.hpp"
#include "header.hpp"
#include <sstream>
#include <cmath>
#include "Channel.hpp"

class Channel;
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
