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
#include "ServerReplies.hpp"

namespace ft
{
	class Server;

	class Protocol
	{
		private:
			typedef void (Protocol::*fncts)(ClientMessage const&);

			static size_t const _MESSAGE_MAX_CHARACTERS;
			static size_t const _MESSAGE_MAX_PARAMETERS;
			static std::string const _IRC_ENDL;

		public:
			Protocol(Server& server);
			~Protocol(void);

			Client& get_client_from_socket(int socket);
			Client& get_client_from_name(std::string const& name);
			Channel& get_channel_from_name(std::string const& name);

			bool is_socket_ignored(int socket) const;
			bool is_client_connected(Client const& client) const;
			bool is_client_active(std::string const& client_name) const;
			bool is_channel_active(std::string const& channel_name) const;
			bool is_valid_nickname(std::string const& nickname) const;
			bool is_channel_name(std::string const& name) const;

			void set_password(std::string const& password);

			void add_client(int socket);
			void delete_client(int socket);

			void ignore_socket(int socket);

			void parse_client_input(int socket, std::string& message);
			void handle_message(ClientMessage const& cmessage);

			void send_message_to_client(Client& client, std::string const& message);
			void send_message_to_channel(Channel const& channel, std::string const& message,
					Client const& sender);
			void send_message_to_client_channels(Client& client, std::string const& message);
			void send_welcome_messages(Client& client);

			// void cmd_invite(ClientMessage msg);
			void cmd_join(ClientMessage const& cmessage);
			// void cmd_kick(ClientMessage msg);
			// void cmd_mode(ClientMessage msg);
			// void cmd_names(ClientMessage msg);
			void cmd_nick(ClientMessage const& cmessage);
			// void cmd_oper(ClientMessage msg);
			void cmd_part(ClientMessage const& cmessage);
			void cmd_pass(ClientMessage const& cmessage);
			void cmd_ping(ClientMessage const& cmessage);
			void cmd_privmsg(ClientMessage const& cmessage);
			void cmd_quit(ClientMessage const& cmessage);
			// void cmd_topic(ClientMessage msg);
			void cmd_user(ClientMessage const& cmessage);
			// void cmd_whois(ClientMessage msg);

		private:
			// Protocol(Protocol const& x);
			// Protocol& operator=(Protocol const& x);

			/*
			void _get_server_operators(void);
			bool _is_valid_mode(std::string const& str, std::string const& modes) const;
			*/

			Server& _server;
			ServerReplies _replies;
			std::string _password;
			std::map<std::string, fncts> _commands;
			std::vector<Client> _clients;
			std::vector<Channel> _channels;
			std::map<std::string, std::string> _operators;
			std::vector<int> _ignored_sockets;
	};
}

#endif
