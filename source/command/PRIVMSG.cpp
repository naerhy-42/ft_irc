#include "Protocol.hpp"

namespace ft
{
    void Protocol::cmd_privmsg(ClientMessage const& cmessage)
    {
        Client* client = cmessage.get_client();
        std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 1)
			send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "PRIVMSG"));
		else if (cmessage.get_remainder().empty())
			send_message_to_client(client, _replies.err_notexttosend(client->get_nickname()));
		else if (!is_channel_active(parameters[0]) && !is_client_active(parameters[0]))
			send_message_to_client(client, _replies.err_nosuchnick(client->get_nickname(), parameters[0]));
		else if (is_channel_name(parameters[0]))
		{
			Channel& channel = get_channel_from_name(parameters[0]);

			if (!channel.has_client(client) || (channel.is_moderated() && !channel.has_talk_privilege(client)))
				send_message_to_client(client, _replies.err_cannotsendtochan(client->get_nickname(), parameters[0]));
			else
			{
				std::string message = ":" + client->get_prefix() + " PRIVMSG " + parameters[0] + " :"
						+ cmessage.get_remainder() + _IRC_ENDL;

				send_message_to_channel(channel, message, client);
			}
		}
		else
		{
			Client* target = get_client_from_name(parameters[0]);
			std::string message = ":" + client->get_prefix() + " PRIVMSG " + parameters[0] + " :"
					+ cmessage.get_remainder() + _IRC_ENDL;

			if (target->is_away())
				send_message_to_client(client, _replies.rpl_away(client->get_nickname(),
						target->get_nickname(), target->get_away_reason()));
			send_message_to_client(target, message);
        }
    }
}
