/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 19:36:25 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/30 19:37:08 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ChannelManager.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

ChannelManager::ChannelManager()
{

}

ChannelManager::~ChannelManager(){

}

void ChannelManager::Part(Client &client, const std::string &channelName, const std::string &reason)
{
	(void)client;
	(void)channelName;
	(void)reason;
	std::cout << "entre a Part de channel Manager" << std::endl;
}

void ChannelManager::partAll(Client &client)
{
	std::map<std::string, ChannelManager>::iterator it;
	for (it = _channel.begin(); it != _channel.end(); ++it)
		Part(client, it->first, "");
}
/*
Channel *ChannelManager::Search(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return (it->second);
	return (0);
}*/
