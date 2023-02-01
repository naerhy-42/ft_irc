#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
	void Protocol::cmd_mode(Message msg)
	{
		Client& client = _get_client_from_socket(msg.get_socket());
		std::vector<std::string> parameters = msg.get_parameters();
		std::string reply;

		if (!_is_client_connected(client))
		{
			reply = err_notregistered(client.get_nickname());
			add_to_queue(client, reply, 0);
		}
		else if (parameters.size() < 1)
		{
			reply = err_needmoreparams(client.get_nickname(), "MODE");
			add_to_queue(client, reply, 0);
		}
		else if (!parameters[0].find_first_of("#")) // if target is a channel
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
		}
	}
}
