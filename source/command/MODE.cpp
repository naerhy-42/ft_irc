#include "../../include/Protocol.hpp"
#include "../../include/Server.hpp"

namespace ft
{
	void Protocol::cmd_mode(Message msg)
	{
		Client& client = _get_client_from_socket(msg.get_socket());
		std::vector<std::string> parameters = msg.get_parameters();
		std::string error;

		if (parameters.size() < 1)
		{
			error = err_needmoreparams(client.get_nickname(), "MODE");
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		// if target is a channel
		if (!parameters[0].find_first_of("#"))
		{
			for (size_t i = 0; i < _channels.size(); i++)
			{
				if (_channels[i]->get_name() == parameters[0])
				{
					Channel channel = _get_channel_from_name(parameters[0]);
					if (parameters.size() == 1)
					{
						// we use error but it is not an error (logic)
						error = rpl_channelmodeis(client.get_nickname(), channel.get_name(), channel.get_modes_str());
						_buffer.add_to_queue(client, error, 0);
						return ;
					}
					if (!_channels[i]->is_operator(client.get_nickname()))
					{
						error = err_chanoprivsneeded(client.get_nickname(), channel.get_name());
						_buffer.add_to_queue(client, error, 0);
						return ;
					}
					if (!_is_valid_mode(parameters[1], "mt"))
					{
						error = err_unknownmode(client.get_nickname(), parameters[1][1]);
						_buffer.add_to_queue(client, error, 0);
						return ;
					}
					channel.set_mode(parameters[1][0], parameters[1][1]);
					// send message to user and all users in channel
					// send message to user and all users in channel
					return ;
				}
			}
			error = err_nosuchchannel(client.get_nickname(), parameters[0]);
			_buffer.add_to_queue(client, error, 0);
			return ;
		}
		else
		{
			if (client.get_nickname() != parameters[0])
			{
				error = err_usersdontmatch(client.get_nickname());
				_buffer.add_to_queue(client, error, 0);
				return ;
			}
			if (parameters.size() == 1)
			{
				// we use error but it is not an error (logic)
				error = rpl_umodeis(client.get_nickname(), client.get_modes_str());
				_buffer.add_to_queue(client, error, 1);
				return ;
			}
			if (!_is_valid_mode(parameters[1], "o"))
			{
				error = err_umodeunknownflag(client.get_nickname());
				_buffer.add_to_queue(client, error, 0);
				return ;
			}
			// if client tries to use +o mod or +/-a mode, we do nothing
			// if client tries to use +o mod or +/-a mode, we do nothing
			client.set_mode(parameters[1][0], parameters[1][1]);
			_buffer.add_to_queue(client, ":" + client.get_nickname() + " MODE "
					+ client.get_nickname() + " :" + parameters[1] + "\r\n", 1);
		}
	}
}
