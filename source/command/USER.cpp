#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_user(Message msg)
    {
        std::vector<std::string> parameters;
        std::string error;
        std::string reply;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (!client.get_password_status())
        {
            error = err_notregistered(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        if (client.get_registration_status())
        {
            error = err_alreadyregistered(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        parameters = msg.get_parameters();

        if (parameters.size() < 3 || parameters[0].empty() || parameters[1].empty() || parameters[2].empty() || msg.get_remainder().empty())
        {
            error = err_needmoreparams(client.get_nickname(), "USER");
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        client.set_username(parameters[0]);
        client.set_hostname(parameters[1]);
        client.set_servername(parameters[2]);
        client.set_real_name(msg.get_remainder());
        client.set_registration_status(true);
        if (_is_client_connected(client))
        {
            reply = rpl_welcome(client.get_nickname(), "42FT_IRC", client.get_nickname(),
                                client.get_username(), client.get_hostname());
            _buffer.add_to_queue(client, reply, 1);
            reply = rpl_yourhost(client.get_nickname(), client.get_hostname(), _server->get_version());
            _buffer.add_to_queue(client, reply, 1);
            reply = rpl_created(client.get_nickname(), _server->get_creation_time());
            _buffer.add_to_queue(client, reply, 1);
            reply = rpl_myinfo(client.get_nickname(), client.get_servername(), _server->get_version(),
                               "TEMP VALUES", "TEMP VALUES", "TEMP VALUES");
            _buffer.add_to_queue(client, reply, 1);
        }
    }
}
