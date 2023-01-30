#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_nick(Message msg)
    {
        std::vector<std::string> parameters;
        std::string nickname;
        std::string error;
        std::string reply;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (!client.get_password_status())
        {
            error = err_notregistered(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        parameters = msg.get_parameters();
        if (parameters.empty() || parameters[0].empty())
        {
            error = err_nonicknamegiven(client.get_nickname());
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        else
            nickname = parameters[0];
        if (nickname.length() > 18 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
        {
            error = err_erroneusnickname(client.get_nickname(), nickname);
            _buffer.add_to_queue(client, error, 0);
            return;
        }
        if (_is_client_connected(client))
        {
            if (_is_nickname_taken(nickname))
            {
                error = err_nicknameinuse(client.get_nickname(), nickname);
                _buffer.add_to_queue(client, error, 0);
                return;
            }
            client.set_nickname(nickname);
            // return replies to inform other members
        }
        else
        {
            if (_is_nickname_taken(nickname))
            {
                error = err_nicknameinuse(client.get_nickname(), nickname);
                _buffer.add_to_queue(client, error, 1);
                return;
            }
            client.set_nickname(nickname);
            client.set_nickname_status(true);
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
}
