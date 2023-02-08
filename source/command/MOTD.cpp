#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	void Protocol::cmd_motd(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else
		{
			std::ifstream file;

			file.open("config/motd.txt", std::ios::in);
			if (!file.is_open())
				send_message_to_client(client, _replies.err_nomotd(client->get_nickname()));
			else
			{
				std::string line;

				send_message_to_client(client, _replies.rpl_motdstart(client->get_nickname(), _server.get_hostname()));
				while (getline(file, line))
					send_message_to_client(client, _replies.rpl_motd(client->get_nickname(), line));
				send_message_to_client(client, _replies.rpl_endofmotd(client->get_nickname()));
				file.close();
			}
		}
	}
}
