#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include "Buffer.hpp"
#include "Client.hpp"
#include "Message.hpp"
#include "Reply.hpp"
#include "Channel.hpp"

namespace ft
{
	class Server;

	class Protocol
	{
	private:
		typedef void (Protocol::*fncts)(Message);

	public:
		Protocol(Server *server);
		~Protocol(void);

		void set_password(std::string const &password);

		void add_client(int socket);
		void delete_client(int socket);

		void add_channel(std::string channel_name);
		void delete_channel(std::string channel_name);
		bool is_valid_channel_name(std::string channel_name);




		void parse_client_input(std::string &client_msg, int client_socket);
		void handle_message(Message msg);

		void cmd_pass(Message msg);
		void cmd_nick(Message msg);
		void cmd_user(Message msg);
		void cmd_privmsg(Message msg);
		void cmd_join(Message msg);
		void cmd_ping(Message msg);
		void cmd_quit(Message msg);
		void cmd_names(Message msg); // does not work
		void cmd_whois(Message msg);
		void cmd_part(Message msg);
		void cmd_oper(Message msg);
		void cmd_mode(Message msg);

	private:
		bool _is_nickname_taken(std::string const& nickname) const;
		bool _is_client_connected(Client client) const;
		Client &_get_client_from_socket(int socket);
		Client &_get_client_from_nickname(const std::string &nickname);
		
		Channel &_get_channel_from_name(const std::string &channel_name);

		void _get_server_operators(void);

		bool _is_valid_mode(std::string const& mode) const;

		static size_t const _message_max_characters;
		static size_t const _message_max_parameters;

		Protocol(Protocol const &x);
		Protocol &operator=(Protocol const &x);

		Server *_server;
		std::string _password;
		std::map<std::string, fncts> _commands;
		std::vector<Client> _clients;
		std::vector<Channel*> _channels;
		Buffer _buffer;
		std::map<std::string, std::string> _server_ops;
	};
}

#endif
