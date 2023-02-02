#include "Protocol.hpp"

namespace ft
{
    void Protocol::cmd_join(ClientMessage const& cmessage)
    {
        Client& client = cmessage.get_client();
        std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (parameters.size() < 1)
        {
			send_message_to_client(client, _replies.err_needmoreparams(client.get_nickname(), "JOIN"));
			ignore_socket(client.get_socket());
        }
		else
		{
			std::string channel_name = parameters[0];
			std::string message = ":" + client.get_prefix() + " JOIN " + channel_name + _IRC_ENDL;

			if (is_channel_active(channel_name))
			{
				Channel& channel = get_channel_from_name(channel_name);
				channel.add_client(&client);
			}
			else
				_channels.push_back(Channel(channel_name, &client));
			send_message_to_client(client, message);
			send_message_to_channel(get_channel_from_name(channel_name), message, client);
			// + send topic
		}
    }
}
