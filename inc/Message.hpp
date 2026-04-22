/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 17:31:15 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/21 17:34:56 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

//networkname = servidor en 001

//Bienvenida + info + se crean cosas
#define RPL_WELCOME(nick, server) "001 " + (nick) + ":Welcome to the INEA Network," + (server)
#define RPL_YOURHOST(nick, server) "002 " + (nick) + ":Your host is " + (server) + "running version IRC42"
#define RPL_CREATED(nick, datetime) "003 " + (nick) + ":This server was created" + (datetime)
#define RPL_MYINFO(nick, server) "004 " + (nick) + (server)
#define RPL_UMODEIS(nick, modes) "221 " +  (nick) + (modes)
#define RPL_CHANNELMODEIS(client, channel, modes) "324 " +  (client) + (channel) + (modes)
#define RPL_NOTOPIC(nick, channel) "331 " + (nick) + (channel) + ":No topic is set"
#define RPL_TOPIC(nick, channel) "332 " + (nick) + (channel) + ":<topic>"
#define RPL_INVITING(nick, channel) "341 " +  (nick) + (channel)

// Errores
#define ERR_NOSUCHNICK(nick) "401 " + (nick) + ":No such nick/channel" 
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + (nick) + (channel) + ":No such channel"
#define ERR_CANNOTSENDTOCHAN(nick, channel) "404 " + (nick) + (channel) + ":Cannot send to channel"
#define ERR_NORECIPIENT(nick, command) "411 " + (nick) + ":No recipient given" (command)"
#define ERR_NOTEXTTOSEND(nick) "412 " + (nick) + ":No text to send"
#define ERR_UNKNOWNCOMMAND(nick, command) "421 " + (nick) + (command) + ":Unknown command"
#define ERR_ERRONEUSNICKNAME(nick) "432 " + (nick) + ":Erroneus nickname"
#define ERR_ERRONEUSNICKNAMETOOLONG(nick) "432 " + (nick) + ":Erroneus nickname too long"
#define ERR_ERRONEUSNICKNAMETOOLONG(nick) "432 " + (nick) + ":Nickname too long"
#define ERR_ERRONEUSNICKNAMEINUSE(nick) "432 " + (nick) + ":Nickname in use"
#define ERR_USERNOTINCHANNEL() "441 " + (nick) + (channel) + ":They aren't on that channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + (nick) + (channel) + ":You're not on that channel"
#define ERR_NOTREGISTERED(nick) "451 " +  (nick) + ":You have not registered"
#define ERR_NEEDMOREPARAMS(nick, command) "461 " + (nick) + (command) + ":Not enough parameters"
#define ERR_ALREADYREGISTERED(nick) "462 " + (nick) + ":You may not reregister"
#define ERR_CHANNELISFULL(nick, channel) "471 " + (nick) + (channel) + ":Cannot join channel (+l)" 
#define ERR_UNKNOWNMODE(nick, modechar) "472 " + (nick) + (modechar) + ":is unknown mode char to me"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + (nick) + (channel) + ":Cannot join channel (+i)"
#define ERR_BADCHANNELKEY(nick, channel) "475 " + (nick) + (channel) + ":Cannot join channel (+k)"
#define ERR_NOCHANMODES(nick, channel) "477 " + (nick) + " " + (channel) + " :Channel doesn't support modes"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " +  (nick) +  (channel) + ":You're not channel operator"
#define ERR_USERSDONTMATCH(nick) "502 " + (nick) + ":Cant change mode for other users" 
#define ERR_INVALIDMODEPARAM(nick, target, modechar, parameter) "696 " + (nick) + (target) + (modechar) + (parameter) + ":Invalid mode parameter"
