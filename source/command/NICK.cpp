#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
    void Protocol::cmd_nick(Message msg)
    {
        std::vector<std::string> parameters = msg.get_parameters();
        std::string nickname;
        std::string reply;
        Client &client = _get_client_from_socket(msg.get_socket());

        if (!client.get_password_status())
        {
            reply = err_notregistered(client.get_nickname());
            _buffer.add_to_queue(client, reply, 0);
        }
		else if (parameters.empty() || parameters[0].empty())
        {
            reply = err_nonicknamegiven(client.get_nickname());
            _buffer.add_to_queue(client, reply, 0);
        }
        else
		{
			nickname = parameters[0];
			if (nickname.length() > 18 || nickname.find_first_of(" ,*?!@.#&()[]") != std::string::npos)
			{
				reply = err_erroneusnickname(client.get_nickname(), nickname);
				_buffer.add_to_queue(client, reply, 0);
			}
			else if (_is_client_connected(client))
			{
				if (_is_nickname_taken(nickname))
				{
					reply = err_nicknameinuse(client.get_nickname(), nickname);
					_buffer.add_to_queue(client, reply, 0);
				}
				else
				{
					std::string old_nickname = client.get_nickname();
					client.set_nickname(nickname);
					_buffer.add_to_queue(client, ":" + old_nickname + " NICK " + nickname + "\r\n", 1);
					// return replies to inform other members
				}
			}
			else
			{
				if (_is_nickname_taken(nickname))
				{
					reply = err_nicknameinuse(client.get_nickname(), nickname);
					_buffer.add_to_queue(client, reply, 1);
				}
				else
				{
					client.set_nickname(nickname);
					client.set_nickname_status(true);
					if (_is_client_connected(client))
						_send_welcome_messages(client);
				}
			}
		}
    }
}
