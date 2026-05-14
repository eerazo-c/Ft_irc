/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhea <arhea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:05:05 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/08 17:57:16 by arhea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Client;
class Server;
class Command;

class Channel {
	private:

		std::string _name;
		std::map<int, Client *> _clients;
		std::map<int, Client *> _operators;
		std::map<int, Client *> _invited;
		std::string _topic;
		bool _modeI;
		bool _modeT;
		std::string _key;
		int _limit;



	public:
		Channel();
		Channel(std::string &name);
		~Channel();

	const std::string& getName() const;
	

	void setClientChannel(Server &server);
	void broadcast(const std::string &msg, Server &server, int epoll_fd);
	void broadcastExcept(Client &sender, const std::string &msg, Server &server, int epoll_fd);
	std::string getUsersList() const;

	bool isMember(Client &client) const;
	void addClient(Client &client);
	void removeClient(Client &client);
	void partAll(Client &client);

	int getClientCount() const;

	bool isOperator(Client &client) const;
	void setOperator(Client &client, bool status);

	bool isInvited(Client &client) const;
	void addInvited(Client &client);
	void removeInvited(Client &client);

	const std::string& getTopic() const;
	void setTopic(const std::string &topic);

	bool getMode(char mode) const;
	void setMode(char mode, bool status);

	const std::string& getKey() const;
	void setKey(const std::string  &key);

	int getLimit() const;
	void setLimit(int limit);


	std::string buildJoinMsg(Client &client, std::string chan);
	std::string buildPartMsg(Client &client, std::string chan, std::string message);
	std::string buildQuitMsg(Client &client, std::string message);
	std::string buildPrivMsg(Client &client, std::string target, std::string message);

	std::string buildNamesMsg(Client &client, std::string chan);
	std::string buildEndNamesMsg(Client &client, std::string chan);

};

#endif
