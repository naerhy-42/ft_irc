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

		// RPL_UMODEIS (221)
		std::string rpl_umodeis(std::string const& client_name, std::string const& modes) const;

		// RPL_AWAY (301)
		std::string rpl_away(std::string const& client_name, std::string const& target_nick, std::string const& reason) const;

		// RPL_UNAWAY (305)
		std::string rpl_unaway(std::string const& client_name) const;

		// RPL_NOWAWAY (306)
		std::string rpl_nowaway(std::string const& client_name) const;

		// RPL_WHOISUSER (311)
		std::string rpl_whoisuser(const std::string &client, const std::string &nick,
								  const std::string &username, const std::string &host,
								  const std::string &realname) const;

		// RPL_WHOISSERVER (312)
		std::string rpl_whoisserver(std::string const& client_name, std::string const& nickname) const;

		// RPL_WHOISOPERATOR (313)
		std::string rpl_whoisoperator(std::string const& client_name, std::string const& nickname) const;

		// RPL_ENDOFWHOIS (318)
		std::string rpl_endofwhois(const std::string &client, const std::string &nick) const;

		// RPL_WHOISCHANNELS (319)
		std::string rpl_whoischannels(const std::string &client, const std::string &nick, const std::string &channels_list) const;

		// RPL_CHANNELMODEIS (324)
		std::string rpl_channelmodeis(std::string const& client_name, std::string const& channel, std::string const& modes) const;

		// RPL_NOTOPIC (331)
		std::string rpl_notopic(std::string const &client_name, std::string const &channel_name) const;

		// RPL_TOPIC (332)
		std::string rpl_topic(std::string const &client_name, std::string const &channel_name, std::string const &topic) const;

		// RPL_INVITING (341)
		std::string rpl_inviting(std::string const& client_name, std::string const& nickname, std::string const& channel) const;

		// RPL_NAMREPLY (353)
		std::string rpl_namreply(std::string const& client_name, std::string const& channel_name,
				std::string const& users_list) const;

		// RPL_ENDOFNAMES (366)
		std::string rpl_endofnames(const std::string &client_name, const std::string &channel_name) const;

		// RPL_MOTD (372)
		std::string rpl_motd(std::string const& client_name, std::string const& line) const;

		// RPL_MOTDSTART (375)
		std::string rpl_motdstart(std::string const& client_name, std::string const& hostname) const;

		// RPL_ENDOFMOTD (376)
		std::string rpl_endofmotd(std::string const& client_name) const;

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

		// ERR_NOTEXTTOSEND (412)
		std::string err_notexttosend(std::string const& client_name) const;

		// ERR_NOMOTD (422)
		std::string err_nomotd(std::string const& client_name) const;

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

		// ERR_USERONCHANNEL (443)
		std::string err_useronchannel(std::string const& client_name, std::string const& nickname,
				std::string const& channel) const;

		// ERR_NOTREGISTERED (451)
		std::string err_notregistered(std::string const &client_name) const;

		// ERR_NEEDMOREPARAMS (461)
		std::string err_needmoreparams(std::string const &client_name, std::string const &command) const;

		// ERR_ALREADYREGISTERED (462)
		std::string err_alreadyregistered(std::string const &client_name) const;

		// ERR_PASSWDMISMATCH (464)
		std::string err_passwdmismatch(std::string const &client_name) const;

		// ERR_UNKNOWNMODE (472)
		std::string err_unknownmode(std::string const& client_name, char mode) const;

		// ERR_NOPRIVILEGES (481)
		std::string err_noprivileges(std::string const& client_name) const;

		// ERR_CHANOPRIVSNEEDED (482)
		std::string err_chanoprivsneeded(std::string const &client_name, std::string const &channel_name) const;

		// ERR_NOOPERHOST (491)
		std::string err_nooperhost(std::string const& client_name) const;

		// ERR_UMODEUNKNOWNFLAG (501)
		std::string err_umodeunknownflag(std::string const& client_name) const;

		// ERR_USERSDONTMATCH (502)
		std::string err_usersdontmatch(std::string const& client_name) const;

	private:
		ServerReplies(ServerReplies const& x);
		ServerReplies& operator=(ServerReplies const& x);

		std::string _server_prefix;
		std::string _endl;
	};
}

#endif
