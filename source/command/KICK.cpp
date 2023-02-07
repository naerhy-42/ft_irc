#include "Protocol.hpp"

namespace ft
{
	void Protocol::cmd_kick(ClientMessage const& cmessage)
    {
        Client *client = cmessage.get_client();
        std::vector<std::string> const& parameters = cmessage.get_parameters();

		std::cout << parameters[0] << std::endl;

        if (!is_client_connected(client))
        {
            send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
            ignore_socket(client->get_socket());
        }
		else if (parameters.size() < 2)
            send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "KICK"));
		else if (!is_channel_active(parameters[0]))
			send_message_to_client(client, _replies.err_nosuchchannel(client->get_nickname(), parameters[0]));
		else
		{
			Channel& channel = get_channel_from_name(parameters[0]);

			if (!channel.has_client(client))
				send_message_to_client(client, _replies.err_notonchannel(client->get_nickname(), parameters[0]));
			else if (!channel.has_client_chanmode(client, 'o') && !client->is_global_operator())
				send_message_to_client(client, _replies.err_chanoprivsneeded(client->get_nickname(), parameters[0]));
			else if (!channel.has_client(get_client_from_name(parameters[1])))
				send_message_to_client(client, _replies.err_usernotinchannel(client->get_nickname(),
						parameters[1], parameters[0]));
			else
			{
				std::string message = ":" + client->get_prefix() + " KICK " + parameters[0] + " "
						+ parameters[1] + " :" + cmessage.get_remainder() + _IRC_ENDL;

				send_message_to_client(client, message);
				send_message_to_channel(channel, message, client);
				get_channel_from_name(parameters[0]).remove_client(get_client_from_name(parameters[1]));
				if (!channel.get_clients().size())
					delete_channel(parameters[0]);
			}
		}
	}
}
