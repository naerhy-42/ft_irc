#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_pass(Message msg)
    {
        std::vector<std::string> parameters;
        std::string error;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (client.get_password_status())
        {
            error = err_alreadyregistered(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        parameters = msg.get_parameters();
        if (parameters.empty())
        {
            error = err_needmoreparams(client.get_nickname(), "PASS");
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        if (parameters[0] != _password)
        {
            error = err_passwdmismatch(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        client.set_password_status(true);
    }
}