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
			Protocol(Server& server, std::string const& hostname);
			~Protocol(void);

			Client* get_client_from_socket(int socket);
			Client* get_client_from_name(std::string const& name);
			Channel& get_channel_from_name(std::string const& name);
			std::string get_enabled_modes(int id) const;
			std::string get_user_channels_list(Client const* client) const;

			bool is_socket_ignored(int socket) const;
			bool is_client_connected(Client const* client) const;
			bool is_client_active(std::string const& client_name) const;
			bool is_channel_active(std::string const& channel_name) const;
			bool is_valid_nickname(std::string const& nickname) const;
			bool is_channel_name(std::string const& name) const;
			bool is_valid_mode(std::string const& mode, int id) const;

			void set_password(std::string const& password);
			void set_global_operators(std::vector<std::string> const& operators);

			void add_client(int socket);
			void delete_client(int socket);

			void add_channel(std::string const& name, Client* client);
			void delete_channel(std::string const& name);

			void remove_client_from_channels(Client const* client);

			void ignore_socket(int socket);

			void parse_client_input(int socket, std::string message);
			void handle_message(ClientMessage const& cmessage);

			void send_message_to_client(Client const* client, std::string const& message);
			void send_message_to_channel(Channel const& channel, std::string const& message,
					Client const* sender);
			void send_message_to_client_channels(Client const* client, std::string const& message);
			void send_welcome_messages(Client const* client);

			// void cmd_invite(ClientMessage msg);
			void cmd_join(ClientMessage const& cmessage);
			void cmd_kick(ClientMessage const& cmessage);
			void cmd_mode(ClientMessage const& cmessage);
			void cmd_motd(ClientMessage const& cmessage);
			// void cmd_names(ClientMessage msg);
			void cmd_nick(ClientMessage const& cmessage);
			void cmd_oper(ClientMessage const& cmessage);
			void cmd_part(ClientMessage const& cmessage);
			void cmd_pass(ClientMessage const& cmessage);
			void cmd_ping(ClientMessage const& cmessage);
			void cmd_privmsg(ClientMessage const& cmessage);
			void cmd_quit(ClientMessage const& cmessage);
			void cmd_topic(ClientMessage const& cmessage);
			void cmd_user(ClientMessage const& cmessage);
			void cmd_whois(ClientMessage const& cmessage);
		    void cmd_die(ClientMessage const& cmessage);


		private:
			Protocol(Protocol const& x);
			Protocol& operator=(Protocol const& x);

			Server& _server;
			ServerReplies _replies;
			std::string _password;
			std::map<std::string, fncts> _commands;
			std::vector<Client*> _clients;
			std::vector<Channel> _channels;
			std::map<std::string, std::string> _global_operators;
			std::string _user_modes;
			std::string _channel_modes;
			std::string _user_chan_modes;
			std::vector<int> _ignored_sockets;
	};
}

#endif
