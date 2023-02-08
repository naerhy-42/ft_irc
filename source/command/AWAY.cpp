#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_away(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else
		{
			std::string const& remainder = cmessage.get_remainder();

			if (remainder.empty())
			{
				if (client->is_away())
				{
					send_message_to_client(client, _replies.rpl_unaway(client->get_nickname()));
					client->get_modes_obj().set_mode('-', 'a');
				}
			}
			else
			{
				if (!client->is_away())
					client->get_modes_obj().set_mode('+', 'a');
				client->set_away_reason(remainder);
				send_message_to_client(client, _replies.rpl_nowaway(client->get_nickname()));
			}
		}
	}
}
