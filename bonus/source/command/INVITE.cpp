#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_invite(ClientMessage const& cmessage)
	{
        Client* client = cmessage.get_client();
        std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 2)
			send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "INVITE"));
		else if (!is_channel_active(parameters[1]))
			send_message_to_client(client, _replies.err_nosuchchannel(client->get_nickname(), parameters[1]));
		else
		{
			Client* target = get_client_from_name(parameters[0]);
			Channel& channel = get_channel_from_name(parameters[1]);

			if (!channel.has_client(client))
				send_message_to_client(client, _replies.err_notonchannel(client->get_nickname(), parameters[1]));
			else if (channel.has_client(target))
				send_message_to_client(client, _replies.err_useronchannel(client->get_nickname(), parameters[0], parameters[1]));
			else
			{
				std::string message = ":" + client->get_prefix() + " INVITE " + parameters[0] + " "
						+ parameters[1] + _IRC_ENDL;
				send_message_to_client(client, _replies.rpl_inviting(client->get_nickname(), parameters[0], parameters[1]));
				send_message_to_client(target, message);
			}
		}
	}
}
