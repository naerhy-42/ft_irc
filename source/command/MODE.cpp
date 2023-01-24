#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
	void Protocol::cmd_mode(Message msg)
	{
		Client& client = _get_client_from_socket(msg.get_socket());
		std::vector<std::string> parameters = msg.get_parameters();
		std::string error;

		if (parameters.size() < 2)
		{
			error = err_needmoreparams(client.get_nickname(), "MODE");
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		if (client.get_nickname() != parameters[0])
		{
			error = err_usersdontmatch(client.get_nickname());
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		if (!_is_valid_mode(parameters[1]))
		{
			error = err_umodeunknownflag(client.get_nickname());
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		// if client tries to use +o mod or +/-a mode, we do nothing
		// if client tries to use +o mod or +/-a mode, we do nothing
		client.set_mode(parameters[1][0], parameters[1][1]);
	}
}
