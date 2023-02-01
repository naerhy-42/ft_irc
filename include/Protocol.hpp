#ifndef __PROTOCOL_HPP__
#define __PROTOCOL_HPP__

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include "Channel.hpp"
#include "Client.hpp"
#include "ClientMessage.hpp"
#include "Reply.hpp"

namespace ft
{
	class Server;

	class Protocol
	{
		private:
			typedef void (Protocol::*fncts)(ClientMessage);

		public:
			Protocol(Server& server);
			~Protocol(void);

			// getters

			void set_password(std::string const& password);

			void add_client(int socket);
			void delete_client(int socket);

			void parse_client_input(int socket, std::string& message);
			void handle_message(ClientMessage const& cmessage);














			/*
			void add_channel(std::string channel_name);
			void delete_channel(std::string channel_name);

			bool is_valid_channel_name(std::string channel_name);





			void cmd_invite(ClientMessage msg);
			void cmd_join(ClientMessage msg);
			void cmd_kick(ClientMessage msg);
			void cmd_mode(ClientMessage msg);
			void cmd_names(ClientMessage msg); // does not work
			void cmd_nick(ClientMessage msg);
			void cmd_oper(ClientMessage msg);
			void cmd_part(ClientMessage msg);
			void cmd_pass(ClientMessage msg);
			void cmd_ping(ClientMessage msg);
			void cmd_privmsg(ClientMessage msg);
			void cmd_quit(ClientMessage msg);
			void cmd_topic(ClientMessage msg);
			void cmd_user(ClientMessage msg);
			void cmd_whois(ClientMessage msg);

			void add_to_queue(Client const& client, std::string const& message, int index);
			void send_replies(void);
			void ignore_socket(int socket);
			bool is_socket_ignored(int socket) const;

			void send_msg_to_channel_clients(Channel const& channel, Client const* client,
					std::string const& message);
			*/

		private:
			// Protocol(Protocol const& x);
			// Protocol& operator=(Protocol const& x);

			/*
			bool _is_nickname_taken(std::string const& nickname) const;
			bool _is_client_connected(Client client) const;
			Client &_get_client_from_socket(int socket);
			Client &_get_client_from_nickname(const std::string &nickname);
			
			bool _channel_exists(std::string const& channel) const;
			Channel* _get_channel_from_name(const std::string &channel_name);

			void _get_server_operators(void);
			bool _is_valid_mode(std::string const& str, std::string const& modes) const;
			void _send_welcome_messages(Client const& client);
			*/

			static size_t const _MESSAGE_MAX_CHARACTERS;
			static size_t const _MESSAGE_MAX_PARAMETERS;
			static std::string const _IRC_ENDL;

			Server& _server;
			std::string _password;
			std::map<std::string, fncts> _commands;
			std::vector<Client> _clients;
			std::vector<Channel> _channels;
			std::map<std::string, std::string> _operators;
		
			// std::vector<int> _ignored_sockets;
			// find a way to implement this
	};
}

#endif
