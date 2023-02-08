#include "ServerReplies.hpp"

namespace ft
{
	ServerReplies::ServerReplies(std::string const &prefix, std::string const &endl)
		: _server_prefix(prefix), _endl(endl) {}

	ServerReplies::~ServerReplies(void) {}

	template <typename T>
	std::string ServerReplies::to_string(T value) const
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

	// RPL_WELCOME (001)
	std::string ServerReplies::rpl_welcome(std::string const &client_name,
										   std::string const &client_prefix) const
	{
		std::string description = ":Welcome to our IRC server made for ft_irc project, " + client_prefix;

		return _server_prefix + " 001 " + client_name + " " + description + _endl;
	}

	// RPL_YOURHOST (002)
	std::string ServerReplies::rpl_yourhost(std::string const &client_name, std::string const &hostname,
											std::string const &version) const
	{
		std::string description = ":Your host is " + hostname + ", running version " + version;

		return _server_prefix + " 002 " + client_name + " " + description + _endl;
	}

	// RPL_CREATED (003)
	std::string ServerReplies::rpl_created(std::string const &client_name, std::string const &date) const
	{
		std::string description = ":This server was created " + date;

		return _server_prefix + " 003 " + client_name + " " + description + _endl;
	}

	// RPL_MYINFO (004)
	std::string ServerReplies::rpl_myinfo(std::string const &client_name, std::string const &hostname,
										  std::string const &version, std::string const &available_user_modes,
										  std::string const &available_channel_modes,
										  std::string const &channel_modes_with_parameter) const
	{
		return _server_prefix + " 004 " + client_name + " " + hostname + " " + version + " " + available_user_modes +
			   " " + available_channel_modes + " " + channel_modes_with_parameter + _endl;
	}

	// RPL_UMODEIS (221)
	std::string ServerReplies::rpl_umodeis(std::string const& client_name, std::string const& modes) const
	{
		return _server_prefix + " 221 " + client_name + " " + modes + _endl;
	}

	// RPL_AWAY (301)
	std::string ServerReplies::rpl_away(std::string const& client_name, std::string const& target_nick,
			std::string const& reason) const
	{
		return _server_prefix + " 301 " + client_name + " " + target_nick + " :" + reason + _endl;
	}

	// RPL_UNAWAY (305)
	std::string ServerReplies::rpl_unaway(std::string const& client_name) const
	{
		std::string description = ":You are no longer marked as being away";

		return _server_prefix + " 305 " + client_name + " " + description + _endl;
	}

	// RPL_NOWAWAY (306)
	std::string ServerReplies::rpl_nowaway(std::string const& client_name) const
	{
		std::string description = ":You have been marked as being away";

		return _server_prefix + " 306 " + client_name + " " + description + _endl;
	}

	// RPL_WHOISUSER (311)
	std::string ServerReplies::rpl_whoisuser(const std::string &client, const std::string &nick,
							  const std::string &username, const std::string &host,
							  const std::string &realname) const
	{
		return _server_prefix + " 311 " + client + " " + nick + " " + username + " " + host + " * :" + realname + _endl;
	}

	// RPL_WHOISSERVER (312)
	std::string ServerReplies::rpl_whoisserver(std::string const& client_name, std::string const& nickname) const
	{
		std::string description = ":irc server for 42 project";
		std::string server_name = _server_prefix.substr(1);

		return _server_prefix + " 312 " + client_name + " " + nickname + " " + server_name + " " + description + _endl;
	}

	// RPL_WHOISOPERATOR (313)
	std::string ServerReplies::rpl_whoisoperator(std::string const& client_name, std::string const& nickname) const
	{
		std::string description = ":is an IRC operator";

		return _server_prefix + " 313 " + client_name + " " + nickname + " " + description + _endl;
	}

	// RPL_ENDOFWHOIS (318)
	std::string ServerReplies::rpl_endofwhois(const std::string &client, const std::string &nick) const
	{
		return _server_prefix + " 318 " + client + " " + nick + " :End of /WHOIS list" + _endl;
	}

	// RPL_WHOISCHANNELS (319)
	std::string ServerReplies::rpl_whoischannels(const std::string &client, const std::string &nick,
			const std::string &channels_list) const
	{
		return _server_prefix + " 319 " + client + " " + nick + " :" + channels_list + _endl;
	}

	// RPL_CHANNELMODEIS (324)
	std::string ServerReplies::rpl_channelmodeis(std::string const& client_name,
			std::string const& channel, std::string const& modes) const
	{
		return _server_prefix + " 324 " + client_name + " " + channel + " " + modes + _endl;
	}

	// RPL_NOTOPIC (331)
	std::string ServerReplies::rpl_notopic(std::string const &client_name, std::string const &channel_name) const
	{
		std::string description = ":No topic is set";

		return _server_prefix + " 331 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// RPL_TOPIC (332)
	std::string ServerReplies::rpl_topic(std::string const &client_name,
			std::string const &channel_name, std::string const &topic) const
	{
		std::string description = ": " + topic;

		return _server_prefix + " 332 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// RPL_INVITING (341)
	std::string ServerReplies::rpl_inviting(std::string const& client_name, std::string const& nickname,
			std::string const& channel) const
	{
		return _server_prefix + " 341 " + client_name + " " + nickname + " " + channel + _endl;
	}

	// RPL_NAMREPLY (353)
	std::string ServerReplies::rpl_namreply(std::string const& client_name, std::string const& channel_name,
			std::string const& users_list) const
	{
		return _server_prefix + " 353 " + client_name + " = " + channel_name + " :" + users_list + _endl;
	}

	// RPL_ENDOFNAMES (366)
	std::string ServerReplies::rpl_endofnames(const std::string &client_name, const std::string &channel_name) const
	{
		return _server_prefix + " 366 " + client_name + " " + channel_name + " :End of /NAMES list" + _endl;
	}

	// RPL_MOTD (372)
	std::string ServerReplies::rpl_motd(std::string const& client_name, std::string const& line) const
	{
		return _server_prefix + " 372 " + client_name + " :- " + line + _endl;
	}

	// RPL_MOTDSTART (375)
	std::string ServerReplies::rpl_motdstart(std::string const& client_name, std::string const& hostname) const
	{
		std::string description = ":- " + hostname + " Message of the day -";

		return _server_prefix + " 375 " + client_name + " " + description + _endl;
	}

	// RPL_ENDOFMOTD (376)
	std::string ServerReplies::rpl_endofmotd(std::string const& client_name) const
	{
		std::string description = ":End of /MOTD command";

		return _server_prefix + " 376 " + client_name + " " + description + _endl;
	}

	// RPL_YOUREOPER (381)
	std::string ServerReplies::rpl_youreoper(std::string const& client_name) const
	{
		std::string description = ":You are now an IRC operator";

		return _server_prefix + " 381 " + client_name + " " + description + _endl;
	}

	// ERR_NOSUCHNICK (401)
	std::string ServerReplies::err_nosuchnick(std::string const &client_name, std::string const &nickname) const
	{
		std::string description = ":No such nick/channel";

		return _server_prefix + " 401 " + client_name + " " + nickname + " " + description + _endl;
		;
	}

	// ERR_NOSUCHCHANNEL (403)
	std::string ServerReplies::err_nosuchchannel(std::string const &client_name, std::string const &channel_name) const
	{
		std::string description = ":No such channel";

		return _server_prefix + " 403 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// ERR_CANNOTSENDTOCHAN (404)
	std::string ServerReplies::err_cannotsendtochan(std::string const &client_name, std::string const &channel_name) const
	{
		std::string description = ":Cannot send to channel";

		return _server_prefix + " 404 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// ERR_NOORIGIN (409)
	std::string ServerReplies::err_noorigin(std::string const &client_name) const
	{
		std::string description = ":No origin specified";

		return _server_prefix + " 409 " + client_name + " " + description + _endl;
	}

	// ERR_NOTEXTTOSEND (412)
	std::string ServerReplies::err_notexttosend(std::string const& client_name) const
	{
		std::string description = ":No text to send";

		return _server_prefix + " 412 " + client_name + " " + description + _endl;
	}

	// ERR_NOMOTD (422)
	std::string ServerReplies::err_nomotd(std::string const& client_name) const
	{
		std::string description = ":MOTD file is missing";

		return _server_prefix + " 422 " + client_name + " " + description + _endl;
	}

	// ERR_NONICKNAMEGIVEN (431)
	std::string ServerReplies::err_nonicknamegiven(std::string const &client_name) const
	{
		std::string description = ":No nickname given";

		return _server_prefix + " 431 " + client_name + " " + description + _endl;
	}

	// ERR_ERRONEUSNICKNAME (432)
	std::string ServerReplies::err_erroneusnickname(std::string const &client_name, std::string const &nickname) const
	{
		std::string description = ":Erroneous nickname";

		return _server_prefix + " 432 " + client_name + " " + nickname + " " + description + _endl;
	}

	// ERR_NICKNAMEINUSE (433)
	std::string ServerReplies::err_nicknameinuse(std::string const &client_name, std::string const &nickname) const
	{
		std::string description = ":Nickname is already in use";

		return _server_prefix + " 433 " + client_name + " " + nickname + " " + description + _endl;
	}

	// ERR_USERNOTINCHANNEL (441)
	std::string ServerReplies::err_usernotinchannel(std::string const &client_name,
			std::string const &target_nickname, std::string const &channel_name) const
	{
		std::string description = ":They aren't on that channel";

		return _server_prefix + " 441 " + client_name + " " + target_nickname + " " + channel_name + " " + description + _endl;
	}

	// ERR_NOTONCHANNEL (442)
	std::string ServerReplies::err_notonchannel(std::string const &client_name, std::string const &channel_name) const
	{
		std::string description = ":You're not on that channel";
		return _server_prefix + " 442 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// ERR_USERONCHANNEL (443)
	std::string ServerReplies::err_useronchannel(std::string const& client_name, std::string const& nickname,
			std::string const& channel) const
	{
		std::string description = ":is already on channel";

		return _server_prefix + " 443 " + client_name + " " + nickname + " " + channel + " " + description + _endl;
	}

	// ERR_NOTREGISTERED (451)
	std::string ServerReplies::err_notregistered(std::string const &client_name) const
	{
		std::string description = ":You have not registered";

		return _server_prefix + " 451 " + client_name + " " + description + _endl;
	}

	// ERR_NEEDMOREPARAMS (461)
	std::string ServerReplies::err_needmoreparams(std::string const &client_name, std::string const &command) const
	{
		std::string description = ":Not enough parameters";

		return _server_prefix + " 461 " + client_name + " " + command + " " + description + _endl;
	}

	// ERR_ALREADYREGISTERED (462)
	std::string ServerReplies::err_alreadyregistered(std::string const &client_name) const
	{
		std::string description = ":You may not reregister";

		return _server_prefix + " 462 " + client_name + " " + description + _endl;
	}

	// ERR_PASSWDMISMATCH (464)
	std::string ServerReplies::err_passwdmismatch(std::string const &client_name) const
	{
		std::string description = ":Password incorrect";

		return _server_prefix + " 464 " + client_name + " " + description + _endl;
	}

	// ERR_UNKNOWNMODE (472)
	std::string ServerReplies::err_unknownmode(std::string const& client_name, char mode) const
	{
		std::string description = ":is unknown mode char to me";

		return _server_prefix + " 472 " + client_name + " " + mode + " " + description + _endl;
	}

	// ERR_NOPRIVILEGES (481)
	std::string ServerReplies::err_noprivileges(std::string const& client_name) const
	{
		std::string description = ":Permission denied - You're not an IRC operator";

		return _server_prefix + " 481 " + client_name + " " + description + _endl;
	}

	// ERR_CHANOPRIVSNEEDED (482)
	std::string ServerReplies::err_chanoprivsneeded(std::string const &client_name, std::string const &channel_name) const
	{
		std::string description = ":You're not channel operator";

		return _server_prefix + " 482 " + client_name + " " + channel_name + " " + description + _endl;
	}

	// ERR_NOOPERHOST (491)
	std::string ServerReplies::err_nooperhost(std::string const& client_name) const
	{
		std::string description = ":No O-lines for your host";

		return _server_prefix + " 491 " + client_name + " " + description + _endl;
	}

	// ERR_UMODEUNKNOWNFLAG (501)
	std::string ServerReplies::err_umodeunknownflag(std::string const& client_name) const
	{
		std::string description = ":Unknown MODE flag";

		return _server_prefix + " 501 " + client_name + " " + description + _endl;
	}

	// ERR_USERSDONTMATCH (502)
	std::string ServerReplies::err_usersdontmatch(std::string const& client_name) const
	{
		std::string description = ":Cannot change mode for other users";

		return _server_prefix + " 502 " + client_name + " " + description + _endl;
	}
}
