#pragma once

#include <iostream>
#include <vector>

class ChannelManager {

	private:
		std::map<std::string, Channel *> _channels;

	public:
	ChannelManager();
	~Channel();
	
	Channel *Search(const std::string &name);
	bool AddChannel(Channel &channel);
	void RemoveChannel(Channel &channel);
	bool Join(Client &client, const std::string &name);
	void Part(Client &client, const std::string &channelName,
		const std::string &reason);
	
	void ChannelManager::partAll(Client &client)
	/*
	void Clean(void);
	bool IsValidName(const std::string &name);
	void Kick(Client *client, const std::string &nick,
		const std::string &channelName, const std::string &reason);
	*/

};
