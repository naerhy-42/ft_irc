#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
    void Protocol::cmd_ping(ClientMessage const& cmessage)
    {
        Client& client = cmessage.get_client();
		std::vector<std::string> parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (!parameters.size())
		{
			send_message_to_client(client, _replies.err_noorigin(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else
		{
			std::string pong_reply = "PONG " + parameters[0] + _IRC_ENDL;

			send_message_to_client(client, pong_reply);
		}
    }
}
