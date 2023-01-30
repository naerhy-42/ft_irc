#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
	void Protocol::cmd_oper(Message msg)
	{
        Client &client = _get_client_from_socket(msg.get_socket());
		std::vector<std::string> parameters = msg.get_parameters();
		std::string reply;

		if (parameters.size() < 2)
		{
			reply = err_needmoreparams(client.get_nickname(), "OPER");
			_buffer.add_to_queue(client, reply, 0);
		}
		else if (!_server_ops.count(parameters[0]))
		{
			reply = err_nooperhost(client.get_nickname());
			_buffer.add_to_queue(client, reply, 0);
		}
		else if (_server_ops[parameters[0]] != parameters[1])
		{
			reply = err_passwdmismatch(client.get_nickname());
			_buffer.add_to_queue(client, reply, 0);
		}
		else
		{
			if (!client.has_mode('o'))
			{
				reply = rpl_youreoper(client.get_nickname());
				_buffer.add_to_queue(client, reply, 1);
				client.set_mode('+', 'o');
			}
		}
	}
}
