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
#define RPL_WELCOME(nick, server) "001" + (nick) + ":Welcome to the INEA Network," + (server)
#define RPL_YOURHOST(nick, server, version) "002" + (nick) + ":Your host is " + (server) + "running version" + (version)
#define RPL_CREATED(server) "003" + (client) + ":This server was created" + (datetime)
#define RPL_MYINFO(server) "004" + (client) + (servername) + (version) + (available user modes) + (available channel modes)"
#define RPL_ISUPPORT(server) "005" + (client) + (1-13 tokens) + ":are supported by this server"

// Errores
#define ERR_NOSUCHNICK(server) "401" + (client) + (nickname) + ":No such nick/channel" 
#define ERR_NOSUCHCHANNEL(server) "403" + (client) + (channel) + ":No such channel"
#define ERR_CANNOTSENDTOCHAN(server) "404" + (client) + (channel) + ":Cannot send to channel"
