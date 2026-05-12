/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhea <arhea@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:48:43 by elerazo-          #+#    #+#             */
/*   Updated: 2026/05/11 00:03:50 by arhea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include <iterator>
#include <locale>
#include <cmath>

Pass::~Pass(){}
void Pass::execute(Client& client, std::vector<std::string> params, Server &server) const{
    std::ostringstream oss;
    std::string errorMsg;

    if (params.empty()){
        oss << ":localhost 461 * PASS :Not enough parameters\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (client.getState() >= Client::PASS_ACCEPTED){
        oss << ":localhost 462 * :You may not reregister\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (params[0] != server.getPass()){
        oss << ":localhost 464 * :Password incorrect\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    client.setState(Client::PASS_ACCEPTED);
    std::cout << "Pass execute: SUCCESS" << std::endl;
}

Nick::~Nick(){}
void Nick::execute(Client& client, std::vector<std::string> params, Server &server) const{
    std::ostringstream oss;
    std::string errorMsg;

    if (client.getState() < Client::PASS_ACCEPTED)
        return;

    if (params.empty()){
        oss << ":localhost 431 * :No nickname given\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::string nick = params[0];
    if (client.getNick() == nick)
        return;

    for (size_t i = 0; i < nick.size(); i++){
        char c = nick[i];
        if (i == 0 && isalpha(c))
            continue;
        if (c == '_' || c == '[' || c == ']' || c == '\\' || c == '^' || c == '{' || c == '}')
            continue;
        if (i != 0 && (isalnum(c) || c == '-'))
            continue;
        oss << ":localhost 432 * " << nick << " :Erroneus nickname\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    
    const std::map<int, Client *>& clients = server.getClients();
    std::map<int, Client *>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it){
        if (it->second->getNick() == nick){
            oss << ":localhost 433 * " << nick << " :Nickname is already in use\r\n";
            errorMsg = oss.str();
            send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
            return;
        }
    }
    
    std::string oldNick = client.getNick(); 
    client.setNick(nick);

    if (client.getState() == Client::REGISTERED) {
        oss << ":" << oldNick << "!" << client.getUser() << "@127.0.0.1 NICK :" << nick << "\r\n";
        std::string nickMsg = oss.str();
        send(client.getFd(), nickMsg.c_str(), nickMsg.size(), 0);

        // (Nota para el futuro: Cuando tengas canales, también tendrás que enviarle 
        // este mismo mensaje a todas las personas que estén en los mismos canales 
        // que este usuario, ¡para que vean que se cambió el nombre!)
    }

    if (!client.getUser().empty() && !client.getRealName().empty() && client.getState() != Client::REGISTERED){
        client.setState(Client::REGISTERED);
        std::string nick = client.getNick();
        std::string user = client.getUser();
        std::string host = "127.0.0.1";
        
        oss << ":localhost 001 " << nick << " :Welcome to the Internet Relay Network " 
            << nick << "!" << user << "@" << host << "\r\n";
            
        std::string welcomeMsg = oss.str();
        send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
    }

    std::cout << "Nick execute: SUCCESS" << std::endl;
}

User::~User(){}
void User::execute(Client& client, std::vector<std::string> params, Server &server) const{
    (void)server;
    std::ostringstream oss;
    std::string errorMsg;

    if (client.getState() < Client::PASS_ACCEPTED)
        return;

    if (params.empty() || params.size() < 4 || params[0].empty() || params[3].empty()){
        oss << ":localhost 461 * USER :Not enough parameters\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (client.getState() == Client::REGISTERED){
        oss << ":localhost 462 * :You may not reregister\r\n";
        errorMsg = oss.str();
        send(client.getFd(), errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    client.setUser(params[0]);
    client.setRealName(params[3]);

    if (!client.getNick().empty()){
        client.setState(Client::REGISTERED);
        std::string nick = client.getNick();
        std::string user = client.getUser();
        std::string host = "127.0.0.1";
        
        oss << ":localhost 001 " << nick << " :Welcome to the Internet Relay Network " 
            << nick << "!" << user << "@" << host << "\r\n";
            
        std::string welcomeMsg = oss.str();
        send(client.getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
    }

    std::cout << "User execute: SUCCESS" << std::endl;
}

Join::~Join()
{

}

void Join::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty())
		return ;
	if (client.getState() != Client::REGISTERED)
		return;

	if (args.size() == 1 && args[0] == "0")
	{
		std::map<std::string, Channel> &channels = server.getChannels();

		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (it->second.isMember(client))
            {
                std::string partMsg = it->second.buildPartMsg(client, it->first, "Leaving");
                it->second.broadcast(partMsg);
                it->second.removeClient(client);
            }
		}
		return;
	}
	
	std::map<std::string, Channel> &channels = server.getChannels();
	
	std::stringstream ss(args[0]);
	std::string chan_name;
    std::stringstream keyStream;
    std::string key;
    
    
    if (args.size() > 1)
    {
        keyStream.str(args[1]);
    }

	while (std::getline(ss, chan_name, ','))
	{
        key = "";
        if (args.size() > 1)
            std::getline(keyStream, key, ',');
		if (chan_name.empty() || chan_name[0] != '#')
		{
			 client.WritePrefix(ERR_NOCREATEHAS(client.getNick(), chan_name));
			continue;
		}

		if (channels.find(chan_name) == channels.end())
			channels.insert(std::make_pair(chan_name, Channel(chan_name)));

		Channel &chan = channels[chan_name];

		if (chan.isMember(client))
			continue;
        
        if (chan.getMode('i') == true && chan.isInvited(client) == false)
        {
            client.WritePrefix(ERR_INVITEONLYCHAN(client.getNick(), chan_name));
            continue;
        }

        if (chan.getMode('k') == true && key != chan.getKey())
        {
            client.WritePrefix(ERR_BADCHANNELKEY(client.getNick(), chan_name));
            continue;
        }

        if (chan.getMode('l') == true && chan.getClientCount() >= chan.getLimit())
        {
            client.WritePrefix(ERR_CHANNELISFULL(client.getNick(), chan_name));
            continue;
        }
            
		chan.addClient(client);

		std::string joinMsg = chan.buildJoinMsg(client, chan_name);

		chan.broadcast(joinMsg);

		std::string namesMsg = chan.buildNamesMsg(client, chan_name);
		std::string endMsg = chan.buildEndNamesMsg(client, chan_name);

		send(client.getFd(), namesMsg.c_str(), namesMsg.size(), 0);
		send(client.getFd(), endMsg.c_str(), endMsg.size(), 0);
	}
}

Part::~Part()
{

}

void Part::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	if (args.empty() || args[0].empty())
		return (client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "Part")));

	std::string message;
	if (args.size() > 1)
		message = args[1];
	else 
		message = "";

	std::string chan;
	std::stringstream ss(args[0]);

	std::map<std::string, Channel> &channels = server.getChannels();

	while (std::getline(ss, chan, ','))
	{
		std::map<std::string, Channel>::iterator it = channels.find(chan);

		if (it == channels.end())
			continue;

		Channel &channel = it->second;

		if (!channel.isMember(client))
			continue;
		std::string partMsg = channel.buildPartMsg(client, chan, message);
		
		channel.broadcast(partMsg);
		channel.removeClient(client);
	}
}

Quit::~Quit()
{

}

void Quit::execute(Client& client, std::vector<std::string> args, Server &server) const
{
	std::string message;

	if (!args.empty())
		message = args[0];
	else 
		message = "";

	std::map<std::string, Channel> &channels = server.getChannels();
	Channel tmp;
	std::string quitMsg = tmp.buildQuitMsg(client, message);

	for (std::map<std::string, Channel>::iterator it = channels.begin();
		 it != channels.end(); ++it)
	{
		Channel &chan = it->second;

		if (chan.isMember(client))
		{
			chan.broadcast(quitMsg);
			chan.removeClient(client);
		}
	}
}

PrivMsg::~PrivMsg(void)
{
}

void PrivMsg::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    if (args.empty())
	{
        client.WritePrefix(ERR_NORECIPIENT(client.getNick(), "PRIVMSG"));
		return ;
	}

    if (args.size() == 1)
	{
        client.WritePrefix(ERR_NOTEXTTOSEND(client.getNick()));
		return ;
	}

    std::string target = args[0];

    std::string message;
    for (size_t i = 1; i < args.size(); i++)
    {
        if (i > 1)
            message += " ";
        message += args[i];
    }
	
	Channel tmp;

	std::string fullMsg = tmp.buildPrivMsg(client, target, message);

	for (std::map<int, Client *>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it)
    {
        if (it->second->getNick() == target)
        {
			send(it->second->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
			return;
        }
    }
 
	std::map<std::string, Channel>& channels = server.getChannels();
    std::map<std::string, Channel>::iterator it = channels.find(target);

    if (it != channels.end())
    {
		Channel& channel = it->second;

        if (!channel.isMember(client))
        {
            client.WritePrefix(ERR_CANNOTSENDTOCHAN(client.getNick(), target));
            return;
        }

        channel.broadcastExcept(client, fullMsg);
        return;
	}

	client.WritePrefix(ERR_NOSUCHNICK(client.getNick(), target));
}



static Client* findClientByNick(Server &server, const std::string &nick)
{
	std::map<int, Client *> &clients = server.getClients();
	std::map<int, Client *>::iterator it;

	for (it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second != NULL && it->second->getNick() == nick)
			return (it->second);
	}
	return (NULL);
}

static std::string buildKickReason(Client &client, const std::vector<std::string> &args)
{
	std::string reason;

	if (args.size() < 3)
		return (client.getNick());
	reason = args[2];
	for (std::size_t i = 3; i < args.size(); ++i)
		reason += " " + args[i];
	return (reason);
}


Kick::~Kick()
{
}

void Kick::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    std::string channelName;
    std::string targetNick;
    std::string reason;
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator channelIt;
    Client *target;

	if (args.size() < 2)
	{
		client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "KICK"));
		return;
	}

	channelName = args[0];
	targetNick = args[1];
	reason = buildKickReason(client, args);
    channelIt = channels.find(channelName);
    if (channelIt == channels.end())
    {
		client.WritePrefix(ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}

	Channel &channel = channelIt->second;

	if (!channel.isMember(client))
	{
		client.WritePrefix(ERR_NOTONCHANNEL(client.getNick(), channelName));
		return;
	}

	if (!channel.isOperator(client))
	{
		client.WritePrefix(ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
		return;
	}

	target = findClientByNick(server, targetNick);

	if (target == NULL)
	{
		client.WritePrefix(ERR_NOSUCHNICK(client.getNick(), targetNick));
		return;
	}

	if (!channel.isMember(*target))
	{
		client.WritePrefix(ERR_USERNOTINCHANNEL(client.getNick(), targetNick, channelName));
        return;
    }

	std::string kickMsg = ":" + client.getNick() + "!" + client.getUser()
		+ "@localhost KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";

	channel.broadcast(kickMsg);
	channel.removeClient(*target);
}

Invite::~Invite()
{
}

void Invite::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    std::string targetNick;
    std::string channelName;
    std::map<std::string, Channel> &channels = server.getChannels();
    std::map<std::string, Channel>::iterator channelIt;
    Client *target;

    if (args.size() < 2)
    {
        client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "INVITE"));
        return;
    }

	targetNick = args[0];
	channelName = args[1];

	target = findClientByNick(server, targetNick);
	if (target == NULL)
	{
        client.WritePrefix(ERR_NOSUCHNICK(client.getNick(), targetNick));
        return;
    }

    channelIt = channels.find(channelName);
    if (channelIt == channels.end())
    {
		client.WritePrefix(ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}

	Channel &channel = channelIt->second;

    if (!channel.isMember(client))
    {
        client.WritePrefix(ERR_NOTONCHANNEL(client.getNick(), channelName));
        return;
    }

    if (channel.getMode('i') && !channel.isOperator(client))
    {
        client.WritePrefix(ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
        return;
    }

    if (channel.isMember(*target))
    {
        client.WritePrefix(ERR_USERONCHANNEL(client.getNick(), targetNick, channelName));
        return;
    }

    channel.addInvited(*target);

	client.WritePrefix(RPL_INVITING(client.getNick(), channelName, targetNick));
	target->WritePrefix(":" + client.getNick() + "!" + client.getUser()
		+ "@localhost INVITE " + targetNick + " :" + channelName);
}

Topic::~Topic()
{
}

void Topic::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    std::string channelName;
	std::string topic;
	std::map<std::string, Channel> &channels = server.getChannels();
	std::map<std::string, Channel>::iterator channelIt;

	if (args.empty())
	{
		client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "TOPIC"));
        return;
    }
    channelName = args[0];

    channelIt = channels.find(channelName);
    if (channelIt == channels.end())
    {
        client.WritePrefix(ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
	}

	Channel &channel = channelIt->second;

	if (!channel.isMember(client))
	{
		client.WritePrefix(ERR_NOTONCHANNEL(client.getNick(), channelName));
        return;
    }

    if (args.size() == 1)
    {
        if(channel.getTopic().empty())
            client.WritePrefix(RPL_NOTOPIC(client.getNick(), channelName));
        else
            client.WritePrefix(RPL_TOPIC(client.getNick(), channelName, channel.getTopic()));
        return;
    }

    topic = args[1];
    for (std::size_t i = 2; i < args.size(); ++i)
	{
		topic += " " + args[i];
	}

	if (channel.getMode('t') && !channel.isOperator(client))
	{
		client.WritePrefix(ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
		return;
	}

	channel.setTopic(topic);
	channel.broadcast(":" + client.getNick() + "!" + client.getUser()
		+ "@localhost TOPIC " + channelName + " :" + topic + "\r\n");
}

static std::string buildChannelModes(const Channel& channel)
{
    std::string modes;

    modes = "+";
    if (channel.getMode('i') == true)
        modes += "i";
    if (channel.getMode('t') == true)
        modes += "t";
    if (channel.getMode('k') == true)
        modes += "k";
    if (channel.getMode('l') == true)
        modes += "l";             
    return (modes);       
}

static bool parsePositiveLimit(const std::string& value, int& limit)
{
    std::stringstream ss;

    if (value.empty() == true)
        return (false);
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        if (*it < '0' || *it > '9')
            return (false);
    }
    ss << value;
    ss >> limit;
    if (ss.fail() == true || limit <= 0)
        return (false);
    return (true);
    
}

Mode::~Mode()
{
}

void Mode::execute(Client& client, std::vector<std::string> args, Server &server) const
{
    std::map<std::string, Channel>::iterator channelIt;
    Channel *channel;
    std::string channelName;
    std::string modeParams;
    
    if (args.size() < 1)
    {
        client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
        return;        
    }
    channelName = args[0];
    channelIt = server.getChannels().find(channelName);
    if (channelIt == server.getChannels().end())
    {
        client.WritePrefix(ERR_NOSUCHCHANNEL(client.getNick(), channelName));
        return;
    }
    channel = &(channelIt->second);
    if (args.size() == 1)
    {
        client.WritePrefix(RPL_CHANNELMODEIS(client.getNick(), channelName,
            buildChannelModes(*channel)));
        return;
    }

    if (channel->isMember(client) == false)
    {
        client.WritePrefix(ERR_NOTONCHANNEL(client.getNick(), channelName));
        return;
    }
    if (channel->isOperator(client) == false)
    {
        client.WritePrefix(ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
        return;
    }

    std::string modeString;
    char action;
    std::size_t paramIndex;
    
    modeString = args[1];
    action = '+';
    if (modeString.empty() == true
	    || (modeString[0] != '+' && modeString[0] != '-'))
    {
	    client.WritePrefix(ERR_UNKNOWNMODE(client.getNick(), modeString));
	    return;
    }
    paramIndex = 2;
    for (std::size_t i = 0; i < modeString.size(); ++i)
    {
        if (modeString[i] == '+' || modeString[i] == '-')
        {
            action = modeString[i];
            continue;
        }
        if (modeString[i] == 'i')
            channel->setMode('i', action == '+');
        else if (modeString[i] == 't')
            channel->setMode('t', action == '+');
        else if (modeString[i] == 'k')
        {
            if (action == '+')
            {
                if (paramIndex >= args.size())
                {
                    client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
                    return;
                }
                if (channel->getMode('k') == true)
                {
                    client.WritePrefix(ERR_KEYSET(client.getNick(), channelName));
                    return;
                }
                channel->setKey(args[paramIndex]);
                modeParams += " " + args[paramIndex];
                ++paramIndex;
            }
            else
                channel->setMode('k', false);
        }
        else if (modeString[i] == 'o')
        {
            Client *target;

            if (paramIndex >= args.size())
            {
                client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
                return;
            }
            target = findClientByNick(server, args[paramIndex]);
            if (target == NULL)
            {
                client.WritePrefix(ERR_NOSUCHNICK(client.getNick(), args[paramIndex]));
                return;
            }
            if (channel->isMember(*target) == false)
            {
                client.WritePrefix(ERR_USERNOTINCHANNEL(client.getNick(),
                    args[paramIndex], channelName));
                return;
            }
            channel->setOperator(*target, action == '+');
            modeParams += " " + args[paramIndex];
            ++paramIndex;
        }
        else if (modeString[i] == 'l')
        {
            int limit;

            if (action == '+')
            {
                if (paramIndex >= args.size())
                {
                    client.WritePrefix(ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
                    return;
                }
                if (parsePositiveLimit(args[paramIndex], limit) == false)
                {
                    client.WritePrefix(ERR_INVALIDMODEPARAM(client.getNick(), channelName,
		                std::string(1, modeString[i]), args[paramIndex]));
                    return;
                }
                channel->setLimit(limit);
                modeParams += " " + args[paramIndex];
                ++paramIndex;
            }
            else
                channel->setMode('l', false);
        }
        else
        {
            client.WritePrefix(ERR_UNKNOWNMODE(client.getNick(),
                std::string(1, modeString[i])));
                return;
        }
    }
    
    channel->broadcast(":" + client.getNick() + "!" + client.getUser()
        + "@localhost MODE " + channelName + " " + modeString
        + modeParams + "\r\n");

    
    
}
