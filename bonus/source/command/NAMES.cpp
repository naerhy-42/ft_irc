#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_names(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
		std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		if (parameters.empty())
			send_message_to_client(client, _replies.rpl_endofnames(client->get_nickname(), "*"));
		else if (!is_channel_active(parameters[0]))
			send_message_to_client(client, _replies.rpl_endofnames(client->get_nickname(), parameters[0]));
		else
		{
			std::string users_list = get_users_in_channel_list(get_channel_from_name(parameters[0]));

			send_message_to_client(client, _replies.rpl_namreply(client->get_nickname(), parameters[0], users_list));
			send_message_to_client(client, _replies.rpl_endofnames(client->get_nickname(), parameters[0]));
		}
	}
}
