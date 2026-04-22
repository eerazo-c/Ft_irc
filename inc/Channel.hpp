/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elerazo- <elerazo-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:44:28 by elerazo-          #+#    #+#             */
/*   Updated: 2026/04/13 17:45:13 by elerazo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CHANNEL_CPP
# define CHANNEL_CPP

#include <iostream>
#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"

class Channel {
	private:
	std::map<std::string, Channel> _channel;

	public:
	Channel(std::string &name);
	~Channel();

	void partAll(Client &client);
};

#endif
