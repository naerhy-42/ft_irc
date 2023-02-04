#ifndef __SERVER_REPLIES_HPP__
#define __SERVER_REPLIES_HPP__

#include <sstream>
#include <string>
#include <vector>

namespace ft
{
	class ServerReplies
	{
	public:
		ServerReplies(std::string const &server_prefix, std::string const &endl);
		~ServerReplies(void);

		template <typename T>
		std::string to_string(T value) const;

		// RPL_WELCOME (001)
		std::string rpl_welcome(std::string const &client_name,
								std::string const &client_prefix) const;

		// RPL_YOURHOST (002)
		std::string rpl_yourhost(std::string const &client_name, std::string const &hostname,
								 std::string const &version) const;

		// RPL_CREATED (003)
		std::string rpl_created(std::string const &client_name, std::string const &date) const;

		// RPL_MYINFO (004)
		std::string rpl_myinfo(std::string const &client_name, std::string const &hostname,
							   std::string const &version, std::string const &available_user_modes,
							   std::string const &available_channel_modes,
							   std::string const &channel_modes_with_parameter) const;

		// RPL_NOTOPIC (331)
		std::string rpl_notopic(std::string const &client_name, std::string const &channel_name) const;

		// RPL_TOPIC (332)
		std::string rpl_topic(std::string const &client_name, std::string const &channel_name, std::string const &topic) const;

		// RPL_YOUREOPER (381)
		std::string rpl_youreoper(std::string const& client_name) const;

		// ERR_NOSUCHNICK (401)
		std::string err_nosuchnick(std::string const &client_name, std::string const &nickname) const;

		// ERR_NOSUCHCHANNEL (403)
		std::string err_nosuchchannel(std::string const &client_name, std::string const &channel_name) const;

		// ERR_CANNOTSENDTOCHAN (404)
		std::string err_cannotsendtochan(std::string const &client_name, std::string const &channel_name) const;

		// ERR_NOORIGIN (409)
		std::string err_noorigin(std::string const &client_name) const;

		// ERR_NONICKNAMEGIVEN (431)
		std::string err_nonicknamegiven(std::string const &client_name) const;

		// ERR_ERRONEUSNICKNAME (432)
		std::string err_erroneusnickname(std::string const &client_name, std::string const &nickname) const;

		// ERR_NICKNAMEINUSE (433)
		std::string err_nicknameinuse(std::string const &client_name, std::string const &nickname) const;

		// ERR_USERNOTINCHANNEL (441)
		std::string err_usernotinchannel(std::string const &client_name, std::string const &target_nickname,
				std::string const &channel_name) const;

		// ERR_NOTONCHANNEL (442)
		std::string err_notonchannel(std::string const &client_name, std::string const &channel_name) const;

		// ERR_NOTREGISTERED (451)
		std::string err_notregistered(std::string const &client_name) const;

		// ERR_NEEDMOREPARAMS (461)
		std::string err_needmoreparams(std::string const &client_name, std::string const &command) const;

		// ERR_ALREADYREGISTERED (462)
		std::string err_alreadyregistered(std::string const &client_name) const;

		// ERR_PASSWDMISMATCH (464)
		std::string err_passwdmismatch(std::string const &client_name) const;

		// ERR_CHANOPRIVSNEEDED (482)
		std::string err_chanoprivsneeded(std::string const &client_name, std::string const &channel_name) const;

		// ERR_NOOPERHOST (491)
		std::string err_nooperhost(std::string const& client_name) const;

			/*
			// RPL_WELCOME (001)
			std::string rpl_welcome(const std::string &client, const std::string &networkname,
									const std::string &nick, const std::string &user,
									const std::string &host);

			// RPL_YOURHOST (002)
			std::string rpl_yourhost(const std::string &client, const std::string &hostname,
									 const std::string &version);

			// RPL_CREATED (003)
			std::string rpl_created(const std::string &client, const std::string &date);

			// RPL_MYINFO (004)
			std::string rpl_myinfo(const std::string &client, const std::string &servername,
								   const std::string &version, const std::string &available_user_modes,
								   const std::string &available_channel_modes,
								   const std::string &channel_modes_with_parameter);

			// RPL_ISSUPPORT (005)
			std::string rpl_isupport(const std::string &client, const std::string &tokens);

			// RPL_BOUNCE (010)
			std::string rpl_bounce(const std::string &client, const std::string &hostname,
								   int port, const std::string &info);

			// RPL_UMODEIS (221)
			std::string rpl_umodeis(const std::string &client, const std::string &user_modes);

			// RPL_LUSERSCHANNELS (254)
			std::string rpl_luserchannels(const std::string &client, int channels);

			// RPL_WHOREPLY (352)
			std::string rpl_whoreply(const std::string &client, const std::string &channel,
									 const std::string &username, const std::string &host,
									 const std::string &server, const std::string &nick,
									 const std::string &flags, int hopcount,
									 const std::string &realname);

			// RPL_ENDOFWHO (315)
			std::string rpl_endofwho(const std::string &client, const std::string &mask);

			// RPL_WHOISREGNICK (307)
			std::string rpl_whoisregnick(const std::string &client, const std::string &nick);

			// RPL_WHOISUSER (311)
			std::string rpl_whoisuser(const std::string &client, const std::string &nick,
									  const std::string &username, const std::string &host,
									  const std::string &realname);

			// RPL_WHOWASUSER (314)
			std::string rpl_whowasuser(const std::string &client, const std::string &nick,
									   const std::string &username, const std::string &host,
									   const std::string &realname);

			// RPL_ENDOFWHOIS (318)
			std::string rpl_endofwhois(const std::string &client, const std::string &nick);

			// RPL_WHOISCHANNELS (319)
			std::string rpl_whoischannels(const std::string &client, const std::string &nick, const std::string &channels);

			// RPL_LISTSTART (321)
			std::string rpl_liststart(const std::string &client);

			// RPL_LIST (322)
			std::string rpl_list(const std::string &client, const std::string &channel, int client_count,
								 const std::string &topic);

			// RPL_LISTEND (323)
			std::string rpl_listend(const std::string &client);

			// RPL_CHANNELMODEIS (324)
			std::string rpl_channelmodeis(std::string const& client, std::string const& channel, std::string const& modes);

			// RPL_INVITELIST (336)
			std::string rpl_invitelist(const std::string &channel);

			// RPL_ENDOFINVITELIST (337)
			std::string rpl_endofinvitelist(const std::string &client);

			// RPL_INVITING (341)
			std::string rpl_inviting(const std::string &nick, const std::string &channel);

			// RPL_BANLIST (367)
			std::string rpl_banlist(const std::string &client, const std::string &channel,
									const std::string &mask, const std::string &who,
									long set_ts);

			// RPL_ENDOFBANLIST (368)
			std::string rpl_endofbanlist(const std::string &client, const std::string &channel);

			// RPL_ENDOFWHOWAS (369)
			std::string rpl_endofwhowas(const std::string &client, const std::string &nick);

			// RPL_YOUREOPER (381)
			std::string rpl_youreoper(const std::string& client);

			// RPL_TIME (391)
			std::string rpl_time(const std::string &client, const std::string &server,
								 const std::string &timestamp, int ts_offset,
								 const std::string &human_readable_time);

			// ERR_UNKNOWNERROR (400)
			std::string err_unknownerror(const std::string &client, const std::string &command,
										 const std::string &subcommand, const std::string &info);

			// ERR_NOSUCHNICK (401)
			std::string err_nosuchnick(const std::string &client, const std::string &nick);

			// ERR_NOSUCHSERVER (402)
			std::string err_nosuchserver(const std::string &client, const std::string &server);

			// ERR_NOSUCHCHANNEL (403)
			std::string err_nosuchchannel(const std::string &client, const std::string &channel);

			// ERR_CANNOTSENDTOCHAN (404)
			std::string err_cannotsendtochan(const std::string &client, const std::string &channel);

			// ERR_TOOMANYCHANNELS (405)
			std::string err_toomanychannels(const std::string &client, const std::string &channel);

			// ERR_WASNOSUCHNICK (406)
			std::string err_wasnosuchnick(const std::string &client, const std::string &nick);

			// ERR_NOORIGIN (409)
			std::string err_noorigin(const std::string &client);

			// ERR_INPUTTOOLONG (417)
			std::string err_inputtoolong(const std::string &client);

			// ERR_UNKNOWNCOMMAND (421)
			std::string err_unknowncommand(const std::string &client, const std::string &command);

			// ERR_NONICKNAMEGIVEN (431)
			std::string err_nonicknamegiven(const std::string &client);

			// ERR_ERRONEUSNICKNAME (432)
			std::string err_erroneusnickname(const std::string &client, const std::string &nick);

			// ERR_NICKNAMEINUSE (433)
			std::string err_nicknameinuse(const std::string &client, const std::string &nick);

			// ERR_USERNOTINCHANNEL (441)
			std::string err_usernotinchannel(const std::string &client, const std::string &nick,
											 const std::string &channel);

			// ERR_NOTONCHANNEL (442)
			std::string err_notonchannel(const std::string &client, const std::string &channel);

			// ERR_USERONCHANNEL (443)
			std::string err_useronchannel(const std::string &client, const std::string &nick, const std::string &channel);

			// ERR_NOTREGISTERED (451)
			std::string err_notregistered(const std::string &client);

			// ERR_NEEDMOREPARAMS (461)
			std::string err_needmoreparams(const std::string &client, const std::string &command);

			// ERR_ALREADYREGISTERED (462)
			std::string err_alreadyregistered(const std::string &client);

			// ERR_PASSWDMISMATCH (464)
			std::string err_passwdmismatch(const std::string &client);

			// ERR_YOUREBANNEDCREEP (465)
			std::string err_yourebannedcreep(const std::string &client);

			// ERR_CHANNELISFULL (471)
			std::string err_channelisfull(const std::string &client, const std::string &channel);

			// ERR_UNKNOWNMODE (472)
			std::string err_unknownmode(const std::string &client, char modechar);

			// ERR_INVITEONLYCHAN (473)
			std::string err_inviteonlychan(const std::string &client, const std::string &channel);

			// ERR_BANNEDFROMCHAN (474)
			std::string err_bannedfromchan(const std::string &client, const std::string &channel);

			// ERR_BADCHANNELKEY (475)
			std::string err_badchannelkey(const std::string &client, const std::string &channel);

			// ERR_NOOPERHOST (491)
			std::string err_nooperhost(const std::string &client);

			// ERR_UMODEUNKNOWNFLAG (501)
			std::string err_umodeunknownflag(const std::string &client);

			// ERR_USERSDONTMATCH (502)
			std::string err_usersdontmatch(const std::string &client);    std::string err_badchannelkey(const std::string &client, const std::string &channel);

			// RPL_NAMREPLY (353)
			std::string rpl_namreply(const std::string &server_name, const std::string &client, const std::string &channel_name,
									 const std::vector<std::string> &user_list);

			// RPL_ENDOFNAMES (366)
			std::string rpl_endofnames(const std::string &client, const std::string &channel);
			*/

			private :
			// ServerReplies(ServerReplies const& x);
			// ServerReplies& operator=(ServerReplies const& x);

			std::string _server_prefix;
		std::string _endl;
	};
}

#endif
