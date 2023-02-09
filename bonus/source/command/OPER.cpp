#include "Protocol.hpp"
#include "Server.hpp"

namespace ft
{
	void Protocol::cmd_oper(ClientMessage const& cmessage)
	{
		Client* client = cmessage.get_client();
		std::vector<std::string> const& parameters = cmessage.get_parameters();

		if (!is_client_connected(client))
		{
			send_message_to_client(client, _replies.err_notregistered(client->get_nickname()));
			ignore_socket(client->get_socket());
		}
		else if (parameters.size() < 2)
			send_message_to_client(client, _replies.err_needmoreparams(client->get_nickname(), "OPER"));
		else if (!_global_operators.count(client->get_nickname()) || client->get_nickname() != parameters[0])
			send_message_to_client(client, _replies.err_nooperhost(client->get_nickname()));
		else if (_global_operators[parameters[0]] != parameters[1])
			send_message_to_client(client, _replies.err_passwdmismatch(client->get_nickname()));
		else
		{
			if (!client->is_global_operator())
			{
				send_message_to_client(client, _replies.rpl_youreoper(client->get_nickname()));
				client->get_modes_obj().set_mode('+', 'o');
			}
		}
	}
}
