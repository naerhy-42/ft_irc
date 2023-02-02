#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	void Protocol::cmd_nick(ClientMessage const& cmessage)
	{
		Client& client = cmessage.get_client();
		std::vector<std::string> parameters = cmessage.get_parameters();

		if (!client.get_password_status())
		{
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (parameters.empty() || parameters[0].empty())
		{
			send_message_to_client(client, _replies.err_nonicknamegiven(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else
		{
			std::string nickname = parameters[0];

			if (nickname.length() > 18 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
			{
				send_message_to_client(client, _replies.err_erroneusnickname(client.get_nickname(), nickname));
				ignore_socket(client.get_socket());
			}
			else if (is_client_connected(client))
			{
				if (is_nickname_already_taken(nickname))
				{
					send_message_to_client(client, _replies.err_nicknameinuse(client.get_nickname(), nickname));
					ignore_socket(client.get_socket());
				}
				else
				{
					std::string message = client.get_prefix() + " NICK " + nickname + _IRC_ENDL;

					client.set_nickname(nickname);
					send_message_to_client(client, message);
					send_message_to_client_channels(client, message);
				}
			}
			else
			{
				if (is_nickname_already_taken(nickname))
				{
					send_message_to_client(client, _replies.err_nicknameinuse(client.get_nickname(), nickname));
					ignore_socket(client.get_socket());
				}
				else
				{
					client.set_nickname(nickname);
					client.set_nickname_status(true);
					if (is_client_connected(client))
						send_welcome_messages(client);
				}
			}
		}
	}
}
