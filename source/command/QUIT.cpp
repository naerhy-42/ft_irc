#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_quit(ClientMessage const& cmessage)
    {
        Client* client = cmessage.get_client();
		std::string client_reply = "ERROR :Closing link" + _IRC_ENDL;

		send_message_to_client(client, client_reply);
		ignore_socket(client->get_socket());
		if (is_client_connected(client))
		{
			std::string message;

			if (!cmessage.get_remainder().empty())
				message = ":" + client->get_prefix() + " QUIT :" + cmessage.get_remainder() + _IRC_ENDL;
			else
				message = ":" + client->get_prefix() + " QUIT" + _IRC_ENDL;
			send_message_to_client_channels(client, message);
		}
		_server.close_socket_connection(client->get_socket());
    }
}
