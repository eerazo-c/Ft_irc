/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhea <arhea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:05:05 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/07 14:14:10 by arhea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Client;

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
	
	void broadcast(const std::string &msg);
	void broadcastExcept(Client &sender, const std::string &msg);
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

};

#endif
