#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
    void Protocol::cmd_user(ClientMessage const& cmessage)
    {
		Client& client = cmessage.get_client();
        std::vector<std::string> parameters = cmessage.get_parameters();

        if (!client.get_password_status())
        {
			send_message_to_client(client, _replies.err_notregistered(client.get_nickname()));
			ignore_socket(client.get_socket());
        }
		else if (client.get_registration_status())
			send_message_to_client(client, _replies.err_alreadyregistered(client.get_nickname()));
		else if (parameters.size() < 3 || parameters[0].empty() || parameters[1].empty()
				|| parameters[2].empty() || cmessage.get_remainder().empty())
			send_message_to_client(client, _replies.err_needmoreparams(client.get_nickname(), "USER"));
		else
		{
			client.set_username(parameters[0]);
			client.set_hostname(parameters[1]);
			client.set_servername(parameters[2]);
			client.set_real_name(cmessage.get_remainder());
			client.set_registration_status(true);
			if (is_client_connected(client))
				send_welcome_messages(client);
		}
    }
}
