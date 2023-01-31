#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

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

		class Reply
		{
			public:
				Reply(Client const& client, std::string const& message);
				
				Client const& client;
				std::string const message;
		};

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
		void cmd_kick(Message msg);
    	void cmd_invite(Message msg);
		void cmd_topic(Message msg);
		void cmd_oper(Message msg);
		void cmd_mode(Message msg);

		void add_to_queue(Client const& client, std::string const& message, int index);
		void send_replies(void);
		void ignore_socket(int socket);
		bool is_socket_ignored(int socket) const;


	private:
		bool _is_nickname_taken(std::string const& nickname) const;
		bool _is_client_connected(Client client) const;
		Client &_get_client_from_socket(int socket);
		Client &_get_client_from_nickname(const std::string &nickname);
		
		bool _channel_exists(std::string const& channel) const;
		Channel* _get_channel_from_name(const std::string &channel_name);

		void _get_server_operators(void);
		bool _is_valid_mode(std::string const& str, std::string const& modes) const;
		void _send_welcome_messages(Client const& client);

		static size_t const _message_max_characters;
		static size_t const _message_max_parameters;

		Protocol(Protocol const &x);
		Protocol &operator=(Protocol const &x);

		Server *_server;
		std::string _password;
		std::map<std::string, fncts> _commands;
		std::vector<Client> _clients;
		std::vector<Channel*> _channels;
		std::map<std::string, std::string> _server_ops;
	
		std::vector<Reply> _queue;
		std::vector<int> _ignored_sockets;
	};
}

#endif
