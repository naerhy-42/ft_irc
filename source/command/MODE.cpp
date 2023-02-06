#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	void Protocol::cmd_mode(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
		std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 1)
			send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "MODE"));
		else if (!is_channel_name(parameters[0]))
		{
			if (client->get_nickname() != parameters[0])
				send_message_to_client(client, _replies.err_usersdontmatch(client->get_nickname()));
			else if (parameters.size() == 1)
				send_message_to_client(client, _replies.rpl_umodeis(client->get_nickname(),
						client->get_modes_obj().get_modes_str()));
			else if (!is_valid_mode(parameters[1], 0))
				send_message_to_client(client, _replies.err_umodeunknownflag(client->get_nickname()));
			else
			{
				std::string message = ":" + client->get_prefix() + " MODE " + parameters[0] + " " + parameters[1] + _IRC_ENDL;

				// set mode - do nothing if has already mode
				send_message_to_client(client, message);
			}
		}
		else
		{
		}
		/*
		{
			if (_channel_exists(parameters[0]))
			{
				Channel* channel = _get_channel_from_name(parameters[0]);
				if (parameters.size() == 1)
				{
					reply = rpl_channelmodeis(client.get_nickname(), channel->get_name(), channel->get_modes_str());
					add_to_queue(client, reply, 0);
				}
				else if (!channel->is_operator(&client))
				{
					reply = err_chanoprivsneeded(client.get_nickname(), channel->get_name());
					add_to_queue(client, reply, 0);
				}
				else if (!_is_valid_mode(parameters[1], "mt"))
				{
					reply = err_unknownmode(client.get_nickname(), parameters[1][1]);
					add_to_queue(client, reply, 0);
				}
				else
				{
					channel->set_mode(parameters[1][0], parameters[1][1]);
					add_to_queue(client, ":" + client.get_nickname() + " MODE "
							+ client.get_nickname() + " :" + parameters[1] + "\r\n", 1);
					// send message to all users in channel
					// send message to all users in channel
				}
			}
			else
			{
				reply = err_nosuchchannel(client.get_nickname(), parameters[0]);
				add_to_queue(client, reply, 0);
			}
		}
		else
		{
			if (client.get_nickname() != parameters[0])
			{
				reply = err_usersdontmatch(client.get_nickname());
				add_to_queue(client, reply, 0);
			}
			else if (parameters.size() == 1)
			{
				reply = rpl_umodeis(client.get_nickname(), client.get_modes_str());
				add_to_queue(client, reply, 1);
			}
			else if (!_is_valid_mode(parameters[1], "oi"))
			{
				reply = err_umodeunknownflag(client.get_nickname());
				add_to_queue(client, reply, 0);
			}
			else
			{
				if (parameters[1] != "+o" && ((parameters[1][0] == '+' && !client.has_mode(parameters[1][1]))
						|| (parameters[1][0] == '-' && client.has_mode(parameters[1][1]))))
				{
					client.set_mode(parameters[1][0], parameters[1][1]);
					add_to_queue(client, ":" + client.get_nickname() + " MODE "
							+ client.get_nickname() + " :" + parameters[1] + "\r\n", 1);
				}
			}
		}*/
	}
}
