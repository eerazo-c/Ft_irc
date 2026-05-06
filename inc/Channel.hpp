/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:05:05 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/01 17:05:21 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <iostream>
#include <vector>
#include <map>

class Client;
class Server;

class Channel {
	private:
	std::string _name;
	std::map<int, Client *> _clients;
//	std::map<int, Client *> _invites;
//	std::map<int, Client *> _operators;

	public:
	Channel();
	Channel(std::string &name);
	~Channel();

	const std::string& getName() const;
	

	void setClientChannel(Server &server);
	void broadcast(const std::string &msg);
	void broadcastExcept(Client &sender, const std::string &msg);
	std::string getUsersList() const;

	bool isMember(Client &client);
	void addClient(Client &client);
	void removeClient(Client &client);
	void partAll(Client &client);
};
