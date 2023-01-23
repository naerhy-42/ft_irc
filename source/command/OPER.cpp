#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
	void Protocol::cmd_oper(Message msg)
	{
        Client &client = _get_client_from_socket(msg.get_socket());
		std::vector<std::string> parameters = msg.get_parameters();
		std::string error;
		std::string reply;

		if (parameters.size() < 2)
		{
			error = err_needmoreparams(client.get_nickname(), "OPER");
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		if (!_server_ops.count(parameters[0]))
		{
			error = err_nooperhost(client.get_nickname());
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		if (_server_ops[parameters[0]] != parameters[1])
		{
			error = err_passwdmismatch(client.get_nickname());
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		reply = rpl_youreoper(client.get_nickname());
		_buffer.add_to_queue(client, reply, 1);
		client.set_global_operator_status(true);
	}
}
