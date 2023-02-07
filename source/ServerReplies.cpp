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
			   " " + available_channel_modes + " [" + channel_modes_with_parameter + "]" + _endl;
	}

	// RPL_UMODEIS (221)
	std::string ServerReplies::rpl_umodeis(std::string const& client_name, std::string const& modes) const
	{
		return _server_prefix + " 221 " + client_name + " " + modes + _endl;
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

	// RPL_NAMREPLY (353)
	std::string ServerReplies::rpl_namreply(std::string const& client_name, Channel const& channel) const
	{
		std::string names_msg = ":" + _server_prefix + " 353 " + client_name + " = " + channel.get_name() + " :";
		std::map<Client const*, Modes>::const_iterator cit;

		for (cit = channel.get_clients().begin(); cit != channel.get_clients().end(); cit++)
		{
			if (channel.has_client_chanmode(cit->first, 'o'))
				names_msg += "@";
			names_msg += cit->first->get_nickname() + " ";
		}
		names_msg.erase(names_msg.size() - 1, 1);
		names_msg += _endl;
		return names_msg;
	}

	// RPL_ENDOFNAMES (366)
	std::string ServerReplies::rpl_endofnames(const std::string &client_name, const std::string &channel_name) const
	{
		return _server_prefix + " 366 " + client_name + " " + channel_name + " :End of /NAMES list" + _endl;
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

	// RPL_WHOISUSER (311)
	std::string ServerReplies::rpl_whoisuser(const std::string &client, const std::string &nick,
							  const std::string &username, const std::string &host,
							  const std::string &realname) const
	{
		return _server_prefix + " 311 " + client + " " + nick + " " + username + " " + host + " * :" + realname + _endl;
	}

	// RPL_WHOISCHANNELS (319)
	std::string ServerReplies::rpl_whoischannels(const std::string &client, const std::string &nick, const std::string &channels) const
	{
		return _server_prefix + " 319 " + client + " " + nick + " :" + channels + _endl;
	}

	// RPL_ENDOFWHOIS (318)
	std::string ServerReplies::rpl_endofwhois(const std::string &client, const std::string &nick) const
	{
		return _server_prefix + " 318 " + client + " " + nick + " :End of /WHOIS list" + _endl;
	}
	/*
	// RPL_WELCOME (001)
	std::string rpl_welcome(const std::string &client, const std::string &networkname,
							const std::string &nick, const std::string &user,
							const std::string &host)
	{
		return _server_prefix + " 001 " + client + " :Welcome to the " + networkname + " Network, " + nick +
			   "[!" + user + "@" + host + "]\r\n";
	}

	// RPL_YOURHOST (002)
	std::string rpl_yourhost(const std::string &client, const std::string &hostname,
							 const std::string &version)
	{
		return _server_prefix + " 002 " + client + " :Your host is " + hostname + ", running version " + version + "\r\n";
	}

	// RPL_CREATED (003)
	std::string rpl_created(const std::string &client, const std::string &date)
	{
		return _server_prefix + " 003 " + client + " :This server was created " + date + "\r\n";
	}

	// RPL_MYINFO (004)
	std::string rpl_myinfo(const std::string &client, const std::string &servername,
						   const std::string &version, const std::string &available_user_modes,
						   const std::string &available_channel_modes,
						   const std::string &channel_modes_with_parameter)
	{
		return _server_prefix + " 004 " + client + " " + servername + " " + version + " " + available_user_modes +
			   " " + available_channel_modes + " [" + channel_modes_with_parameter + "]\r\n";
	}

	// RPL_ISSUPPORT (005)
	std::string rpl_isupport(const std::string &client, const std::string &tokens)
	{
		return _server_prefix + " 005 " + client + " " + tokens + " :are supported by this server\r\n";
	}

	// RPL_BOUNCE (010)
	std::string rpl_bounce(const std::string &client, const std::string &hostname,
						   int port, const std::string &info)
	{
		return _server_prefix + " 010 " + client + " " + hostname + " " + to_string(port) + " :" + info + "\r\n";
	}

	// RPL_UMODEIS (221)
	std::string rpl_umodeis(const std::string &client, const std::string &user_modes)
	{
		return _server_prefix + " 221 " + client + " " + user_modes + "\r\n";
	}

	// RPL_LUSERSCHANNELS (254)
	std::string rpl_luserchannels(const std::string &client, int channels)
	{
		return _server_prefix + " 254 " + client + " " + to_string(channels) + " :channels formed\r\n";
	}

	// RPL_WHOREPLY (352)
	std::string rpl_whoreply(const std::string &client, const std::string &channel,
							 const std::string &username, const std::string &host,
							 const std::string &server, const std::string &nick,
							 const std::string &flags, int hopcount,
							 const std::string &realname)
	{
		return _server_prefix + " 352 " + client + " " + channel + " " + username + " " + host + " " +
			   server + " " + nick + " " + flags + " " + to_string(hopcount) +
			   " " + realname + "\r\n";
	}

	// RPL_NAMREPLY (353)
	std::string rpl_namreply(const std::string &server_name, const std::string &client, const std::string &channel_name,
							 const std::vector<std::string> &user_list)
	{
		std::string names_msg = ":" + server_name + " 353 " + client + " = " + channel_name + " :";
		for (size_t i = 0; i < user_list.size(); i++)
		{
			names_msg += user_list[i];
			if (i != user_list.size() - 1)
			{
				names_msg += " ";
			}
		}
		names_msg += "\r\n";
		return names_msg;
	}

	// RPL_ENDOFNAMES (366)
	std::string rpl_endofnames(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 366 " + client + " " + channel + " : End of /NAMES list\r\n";
	}

	// RPL_ENDOFWHO (315)
	std::string rpl_endofwho(const std::string &client, const std::string &mask)
	{
		return _server_prefix + " 315 " + client + " " + mask + " :End of WHO list\r\n";
	}

	// RPL_WHOISREGNICK (307)
	std::string rpl_whoisregnick(const std::string &client, const std::string &nick)
	{
		return _server_prefix + " 307 " + client + " " + nick + " :has identified for this nick\r\n";
	}

	// RPL_WHOWASUSER (314)
	std::string rpl_whowasuser(const std::string &client, const std::string &nick,
							   const std::string &username, const std::string &host,
							   const std::string &realname)
	{
		return _server_prefix + " 314 " + client + " " + nick + " " + username + " " + host + " * :" + realname + "\r\n";
	}





	// RPL_LISTSTART (321)
	std::string rpl_liststart(const std::string &client)
	{
		return _server_prefix + " 321 " + client + " Channel :Users Name\r\n";
	}

	// RPL_LIST (322)
	std::string rpl_list(const std::string &client, const std::string &channel, int client_count,
						 const std::string &topic)
	{
		return _server_prefix + " 322 " + client + " " + channel + " " + to_string(client_count) + " :" + topic + "\r\n";
	}

	// RPL_LISTEND (323)
	std::string rpl_listend(const std::string &client)
	{
		return _server_prefix + " 323 " + client + " :End of /LIST\r\n";
	}

	// RPL_CHANNELMODEIS (324)
	std::string rpl_channelmodeis(std::string const& client, std::string const& channel, std::string const& modes)
	{
		return _server_prefix + " 324 " + client + " " + channel + " " + modes + "\r\n";
	}

	// RPL_INVITELIST (336)
	std::string rpl_invitelist(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 336 " + client + " " + channel + "\r\n";
	}

	// RPL_ENDOFINVITELIST (337)
	std::string rpl_endofinvitelist(const std::string &client)
	{
		return _server_prefix + " 337 " + client + " :End of /INVITE list\r\n";
	}

	// RPL_INVITING (341)
	std::string rpl_inviting(const std::string &nick, const std::string &channel)
	{
		return _server_prefix + " 341 " + nick + " " + channel + "\r\n";
	}

	// RPL_BANLIST (367)
	std::string rpl_banlist(const std::string &client, const std::string &channel,
							const std::string &mask, const std::string &who,
							long set_ts)
	{
		std::string message = _server_prefix + " 367 " + client + " " + channel + " " + mask;
		if (!who.empty())
			message += " " + who;
		if (set_ts > 0)
			message += " " + to_string(set_ts);
		return message + "\r\n";
	}

	// RPL_ENDOFBANLIST (368)
	std::string rpl_endofbanlist(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 368 " + client + " " + channel + " :End of channel ban list\r\n";
	}

	// RPL_ENDOFWHOWAS (369)
	std::string rpl_endofwhowas(const std::string &client, const std::string &nick)
	{
		return client + " " + nick + " :End of WHOWAS\r\n";
	}

	// RPL_YOUREOPER (381)
	std::string rpl_youreoper(const std::string &client)
	{
		return _server_prefix + " 381 " + client + " :You are now an IRC operator\r\n";
	}

	// RPL_TIME (391)
	std::string rpl_time(const std::string &client, const std::string &server,
						 const std::string &timestamp, int ts_offset,
						 const std::string &human_readable_time)
	{
		std::string message = _server_prefix + " 391 " + client + " " + server + " ";
		if (!timestamp.empty())
		{
			message += timestamp + " ";
		}
		if (ts_offset != 0)
		{
			message += to_string(ts_offset) + " ";
		}
		message += ":" + human_readable_time;
		return message + "\r\n";
	}

	// ERR_UNKNOWNERROR (400)
	std::string err_unknownerror(const std::string &client, const std::string &command,
								 const std::string &subcommand, const std::string &info)
	{
		std::string message = _server_prefix + " 400 " + client + " " + command;
		if (!subcommand.empty())
		{
			message += " " + subcommand;
		}
		message += " :" + info;
		return message + "\r\n";
	}

	// ERR_NOSUCHNICK (401)
	std::string err_nosuchnick(const std::string &client, const std::string &nickname)
	{
		return _server_prefix + " 401 " + client + " " + nickname + " :No such nick/channel\r\n";
	}

	// ERR_NOSUCHSERVER (402)
	std::string err_nosuchserver(const std::string &client, const std::string &server_name)
	{
		return _server_prefix + " 402 " + client + " " + server_name + " :No such server\r\n";
	}

	// ERR_NOSUCHCHANNEL (403)
	std::string err_nosuchchannel(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 403 " + client + " " + channel + " :No such channel\r\n";
	}

	// ERR_CANNOTSENDTOCHAN (404)
	std::string err_cannotsendtochan(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 404 " + client + " " + channel + ":Cannot send to channel\r\n";
	}

	// ERR_TOOMANYCHANNELS (405)
	std::string err_toomanychannels(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 405 " + client + " " + channel + " :You have joined too many channels\r\n";
	}

	// ERR_WASNOSUCHNICK (406)
	std::string err_wasnosuchnick(const std::string &client, const std::string &nick)
	{
		return _server_prefix + " 406 " + client + " " + nick + " :There was no such nickname\r\n";
	}

	// ERR_NOORIGIN (409)
	std::string err_noorigin(const std::string &client)
	{
		return _server_prefix + " 409 " + client + " :No origin specified\r\n";
	}

	// ERR_INPUTTOOLONG (417)
	std::string err_inputtoolong(const std::string &client)
	{
		return _server_prefix + " 417 " + client + " :Input line was too long\r\n";
	}

	// ERR_UNKNOWNCOMMAND (421)
	std::string err_unknowncommand(const std::string &client, const std::string &command)
	{
		return _server_prefix + " 421 " + client + " " + command + " :Unknown command\r\n";
	}

	// ERR_NONICKNAMEGIVEN (431)
	std::string err_nonicknamegiven(const std::string &client)
	{
		return _server_prefix + " 431 " + client + " :No nickname given\r\n";
	}

	// ERR_ERRONEUSNICKNAME (432)
	std::string err_erroneusnickname(const std::string &client, const std::string &nick)
	{
		return _server_prefix + " 432 " + client + " " + nick + " :Erroneous nickname\r\n";
	}

	// ERR_NICKNAMEINUSE (433)
	std::string err_nicknameinuse(const std::string &client, const std::string &nick)
	{
		return _server_prefix + " 433 " + client + " " + nick + " :Nickname is already in use\r\n";
	}

	// ERR_USERNOTINCHANNEL (441)
	std::string err_usernotinchannel(const std::string &client, const std::string &nick,
									 const std::string &channel)
	{
		return _server_prefix + " 441 " + client + " " + nick + " " + channel + " :They aren't on that channel\r\n";
	}

	// ERR_NOTONCHANNEL (442)
	std::string err_notonchannel(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 442 " + client + " " + channel + " :You're not on that channel\r\n";
	}

	// ERR_USERONCHANNEL (443)
	std::string err_useronchannel(const std::string &client, const std::string &nick, const std::string &channel)
	{
		return _server_prefix + " 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n";
	}

	// ERR_NOTREGISTERED (451)
	std::string err_notregistered(const std::string &client)
	{
		return _server_prefix + " 451 " + client + " :You have not registered\r\n";
	}

	// ERR_NEEDMOREPARAMS (461)
	std::string err_needmoreparams(const std::string &client, const std::string &command)
	{
		return _server_prefix + " 461 " + client + " " + command + " :Not enough parameters\r\n";
	}

	// ERR_ALREADYREGISTERED (462)
	std::string err_alreadyregistered(const std::string &client)
	{
		return _server_prefix + " 462 " + client + " :You may not reregister\r\n";
	}

	// ERR_PASSWDMISMATCH (464)
	std::string err_passwdmismatch(const std::string &client)
	{
		return _server_prefix + " 464 " + client + " :Password incorrect\r\n";
	}

	// ERR_YOUREBANNEDCREEP (465)
	std::string err_yourebannedcreep(const std::string &client)
	{
		return _server_prefix + " 465 " + client + " :You are banned from this server.\r\n";
	}

	// ERR_CHANNELISFULL (471)
	std::string err_channelisfull(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n";
	}

	// ERR_UNKNOWNMODE (472)
	std::string err_unknownmode(const std::string &client, char modechar)
	{
		std::string message = _server_prefix + " 472 " + client + " " + modechar + " :is unknown mode char to me\r\n";
		return message;
	}

	// ERR_INVITEONLYCHAN (473)
	std::string err_inviteonlychan(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n";
	}

	// ERR_BANNEDFROMCHAN (474)
	std::string err_bannedfromchan(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 474 " + client + " " + channel + " :Cannot join channel (+b)\r\n";
	}

	// ERR_BADCHANNELKEY (475)
	std::string err_badchannelkey(const std::string &client, const std::string &channel)
	{
		return _server_prefix + " 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n";
	}

	// ERR_CHANOPRIVSNEEDED (482)
	std::string err_chanoprivsneeded(std::string const& client, std::string const& channel)
	{
		return _server_prefix + " 482 " + client + " " + channel + " :You're not channel operator\r\n";
	}

	// ERR_NOOPERHOST (491)
	std::string err_nooperhost(const std::string &client)
	{
		return _server_prefix + " 491 " + client + " :No O-lines for your host\r\n";
	}

	// ERR_UMODEUNKNOWNFLAG (501)
	std::string err_umodeunknownflag(const std::string &client)
	{
		return _server_prefix + " 501 " + client + " :Unknown MODE flag\r\n";
	}

	// ERR_USERSDONTMATCH (502)
	std::string err_usersdontmatch(const std::string &client)
	{
		return _server_prefix + " 502 " + client + " :Cannot change mode for other users\r\n";
	}

	std::string rpl_topic(const std::string &channel, const std::string &topic)
	{
		return _server_prefix + " 332 " + channel + " :" + topic + "\r\n";
	}
	*/
}
