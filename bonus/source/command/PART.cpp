#include "Protocol.hpp"

namespace ft 
{
	void Protocol::cmd_part(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
		std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 1)
			send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "PART"));
		else if (!is_channel_active(parameters[0]))
			send_message_to_client(client, _replies.err_nosuchchannel(client->get_nickname(), parameters[0]));
		else if (!get_channel_from_name(parameters[0]).has_client(client))
			send_message_to_client(client, _replies.err_notonchannel(client->get_nickname(), parameters[0]));
		else
		{
			Channel& channel = get_channel_from_name(parameters[0]);

			std::string message = ":" + client->get_prefix() + " PART " + parameters[0] + " :" + cmessage.get_remainder() + _IRC_ENDL;
			send_message_to_client(client, message);
			send_message_to_channel(channel, message, client);
			channel.remove_client(client);
			if (!channel.get_clients().size())
				delete_channel(parameters[0]);
		}
	}
}
