#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	void Protocol::cmd_nick(ClientMessage const& cmessage)
	{
		Client& client = cmessage.get_client();
		std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!client.get_password_status())
		{
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (parameters.size() < 1)
			send_message_to_client(client, _replies.err_nonicknamegiven(client.get_nickname()));
		else
		{
			std::string nickname = parameters[0];

			if (!is_valid_nickname(nickname))
				send_message_to_client(client, _replies.err_erroneusnickname(client.get_nickname(), nickname));
			else if (is_client_active(nickname))
				send_message_to_client(client, _replies.err_nicknameinuse(client.get_nickname(), nickname));
			else
			{
				client.set_nickname(nickname);
				if (is_client_connected(client))
				{
					std::string message = ":" + client.get_prefix() + " NICK " + nickname + _IRC_ENDL;

					send_message_to_client(client, message);
					send_message_to_client_channels(client, message);
				}
				else
				{
					client.set_nickname_status(true);
					if (is_client_connected(client))
						send_welcome_messages(client);
				}
			}
		}
	}
}
