#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
    void Protocol::cmd_pass(ClientMessage const& cmessage)
    {
		Client& client = cmessage.get_client();
        std::vector<std::string> const& parameters = cmessage.get_parameters();

        if (client.get_password_status())
		{
			send_message_to_client(client, _replies.err_alreadyregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else if (parameters.empty())
		{
			send_message_to_client(client, _replies.err_needmoreparams(client.get_nickname(), "PASS"));
			ignore_socket(client.get_socket());
		}
		else if (parameters[0] != _password)
		{
			send_message_to_client(client, _replies.err_passwdmismatch(client.get_nickname()));
			ignore_socket(client.get_socket());
		}
		else
			client.set_password_status(true);
    }
}
