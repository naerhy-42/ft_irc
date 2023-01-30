#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_pass(Message msg)
    {
        std::vector<std::string> parameters = msg.get_parameters();
        std::string reply;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (client.get_password_status())
        {
            reply = err_alreadyregistered(client.get_nickname());
            _buffer.add_to_queue(client, reply, -1);
        }
		else if (parameters.empty())
        {
            reply = err_needmoreparams(client.get_nickname(), "PASS");
            _buffer.add_to_queue(client, reply, -1);
        }
		else if (parameters[0] != _password)
        {
            reply = err_passwdmismatch(client.get_nickname());
            _buffer.add_to_queue(client, reply, -1);
        }
		else
			client.set_password_status(true);
    }
}
