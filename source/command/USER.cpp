#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_user(Message msg)
    {
        std::vector<std::string> parameters = msg.get_parameters();
        std::string reply;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (!client.get_password_status())
        {
            reply = err_notregistered(client.get_nickname());
            add_to_queue(client, reply, 0);
        }
		else if (client.get_registration_status())
        {
            reply = err_alreadyregistered(client.get_nickname());
            add_to_queue(client, reply, 0);
        }
		else if (parameters.size() < 3 || parameters[0].empty() || parameters[1].empty()
				|| parameters[2].empty() || msg.get_remainder().empty())
        {
            reply = err_needmoreparams(client.get_nickname(), "USER");
            add_to_queue(client, reply, 0);
        }
		else
		{
			client.set_username(parameters[0]);
			client.set_hostname(parameters[1]);
			client.set_servername(parameters[2]);
			client.set_real_name(msg.get_remainder());
			client.set_registration_status(true);
			if (_is_client_connected(client))
				_send_welcome_messages(client);
		}
    }
}
