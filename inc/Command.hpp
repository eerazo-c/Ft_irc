/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:00:40 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/20 14:07:05 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <iostream>

class Command 
{
	protected:
		std::string typeCmd;

	public:
		Command();
		Command(const Command &obj);
		Command &operator=(const Command &obj);

		Command(std::string cmd);
		virtual ~Command();

		std::string getCommand(void)const;
		virtual void execute(void) const;
		//Command Cp(void);
};

class Pass : public Command
{
	void execute(void) const{
		std::cout << "execute pass" << std::endl;
	}
};

class Nick : public Command
{
	void execute(void) const{
		std::cout << "execute Nick" << std::endl;
	}
};

class User : public Command
{
	void execute(void) const{
		std::cout << "execute user" << std::endl;
	}
};

class Join : public Command
{
	void execute(void) const{
		std::cout << "execute join" << std::endl;
	}
};

class Part : public Command
{
	void execute(void) const{
		std::cout << "execute part" << std::endl;

	}
};

class Quit : public Command
{
	void execute(void) const{
		std::cout << "execute Quit" << std::endl;
	}
};
