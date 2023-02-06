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
				Modes& modes = client->get_modes_obj();

				if (parameters[1] != "+o" && ((parameters[1][0] == '+'
						&& !modes.has_mode(parameters[1][1])) || (parameters[1][0] == '-'
						&& modes.has_mode(parameters[1][1]))))
				{
					modes.set_mode(parameters[1][0], parameters[1][1]);
					send_message_to_client(client, message);
				}
			}
		}
		else
		{
			if (!is_channel_active(parameters[0]))
				send_message_to_client(client, _replies.err_nosuchchannel(client->get_nickname(), parameters[0]));
			else if (parameters.size() == 1)
				send_message_to_client(client, _replies.rpl_channelmodeis(client->get_nickname(),
						parameters[0], get_channel_from_name(parameters[0]).get_modes_obj().get_modes_str()));
			else if (!get_channel_from_name(parameters[0]).has_client_chanmode(client, 'o') && !client->get_modes_obj().has_mode('o'))
				send_message_to_client(client, _replies.err_chanoprivsneeded(client->get_nickname(), parameters[0]));
			else if (!is_valid_mode(parameters[1], 1))
				send_message_to_client(client, _replies.err_unknownmode(client->get_nickname(), parameters[1][1]));
			else
			{
				std::string message;
				Channel& channel = get_channel_from_name(parameters[0]);
				Modes& modes = channel.get_modes_obj();

				if (parameters.size() == 2)
				{
					message = ":" + client->get_prefix() + " MODE " + parameters[0] + " "
							+ parameters[1] + _IRC_ENDL;

					if ((parameters[1][0] == '+' && !modes.has_mode(parameters[1][1]))
							|| (parameters[1][0] == '-' && modes.has_mode(parameters[1][1])))
					{
						modes.set_mode(parameters[1][0], parameters[1][1]);
						send_message_to_client(client, message);
						send_message_to_channel(channel, message, client);
					}
				}
				else
				{
					Client* target = get_client_from_name(parameters[2]);

					message = ":" + client->get_prefix() + " MODE " + parameters[0] + " "
							+ parameters[1] + " " + parameters[2] + _IRC_ENDL;

					if ((parameters[1][0] == '+' && !channel.has_client_chanmode(target, parameters[1][1]))
							|| (parameters[1][0] == '-' && channel.has_client_chanmode(target, parameters[1][1])))
					{
						channel.set_client_chanmode(target, parameters[1][0], parameters[1][1]);
						send_message_to_client(client, message);
						send_message_to_channel(channel, message, client);
					}
				}
			}
		}
	}
}
