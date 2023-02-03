#include "Protocol.hpp"

namespace ft
{
    void Protocol::cmd_privmsg(ClientMessage const& cmessage)
    {
        Client& client = cmessage.get_client();
        std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (parameters.size() != 2)
			send_message_to_client(client, _replies.err_needmoreparams(client.get_nickname(), "PRIVMSG"));
		else if (is_channel_name(parameters[0]))
		{
			if (!is_channel_active(parameters[0]))
				send_message_to_client(client, _replies.err_nosuchnick(client.get_nickname(), parameters[0]));
			else if (!get_channel_from_name(parameters[0]).has_client(&client))
				send_message_to_client(client, _replies.err_cannotsendtochan(client.get_nickname(), parameters[0]));
			else
			{
				std::string message = ":" + client.get_prefix() + " PRIVMSG " + parameters[0] + " :"
						+ cmessage.get_remainder() + _IRC_ENDL;
				send_message_to_channel(get_channel_from_name(parameters[0]), message, client);
			}
		}
		else
		{
			if (!is_client_active(parameters[0]))
				send_message_to_client(client, _replies.err_nosuchnick(client.get_nickname(), parameters[0]));
			else
			{
				std::string message = ":" + client.get_prefix() + " PRIVMSG " + parameters[0] + " :"
						+ cmessage.get_remainder() + _IRC_ENDL;
				send_message_to_client(get_client_from_name(parameters[0]), message);
			}
        }
    }
}
