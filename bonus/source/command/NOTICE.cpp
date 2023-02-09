#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_notice(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
        std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
			ignore_socket(client->get_socket());
		else if (parameters.size() >= 1 && !cmessage.get_remainder().empty()
				&& (is_channel_active(parameters[0]) || is_client_active(parameters[0])))
		{
			if (is_channel_name(parameters[0]))
			{
				Channel& channel = get_channel_from_name(parameters[0]);

				if (channel.has_client(client)
						&& (!channel.is_moderated() || channel.has_talk_privilege(client)))
				{
					std::string message = ":" + client->get_prefix() + " NOTICE " + parameters[0]
							+ " :" + cmessage.get_remainder() + _IRC_ENDL;

					send_message_to_channel(channel, message, client);
				}
			}
			else
			{
				Client* target = get_client_from_name(parameters[0]);
				std::string message = ":" + client->get_prefix() + " NOTICE " + parameters[0]
						+ " :" + cmessage.get_remainder() + _IRC_ENDL;

				send_message_to_client(target, message);
			}
		}
		else { /* no action is needed */ }
	}
}
