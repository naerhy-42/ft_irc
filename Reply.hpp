#ifndef __REPLY_HPP__
#define __REPLY_HPP__

#include <string>
#include <map>
#include <vector>

namespace ft
{
    // RPL_WELCOME (001)
    std::string rpl_welcome(const std::string &client, const std::string &networkname,
                            const std::string &nick, const std::string &user,
                            const std::string &host)
    {
        return client + " :Welcome to the " + networkname + " Network, " + nick +
               "[!" + user + "@" + host + "]";
    }

    // RPL_YOURHOST (002)
    std::string rpl_yourhost(const std::string &client, const std::string &hostname,
                             const std::string &version)
    {
        return client + " :Your host is " + hostname + ", running version " + version;
    }

    // RPL_CREATED (003)
    std::string rpl_created(const std::string &client, const std::string &date)
    {
        return client + " :This server was created " + date;
    }

    // RPL_MYINFO (004)
    std::string rpl_myinfo(const std::string &client, const std::string &servername,
                           const std::string &version, const std::string &available_user_modes,
                           const std::string &available_channel_modes,
                           const std::string &channel_modes_with_parameter)
    {
        return client + " " + servername + " " + version + " " + available_user_modes +
               " " + available_channel_modes + " [" + channel_modes_with_parameter + "]";
    }

    // RPL_ISSUPPORT (005)
    std::string rpl_isupport(const std::string &client, const std::string &tokens)
    {
        return client + " " + tokens + " :are supported by this server";
    }

    // RPL_BOUNCE (010)
    std::string rpl_bounce(const std::string &client, const std::string &hostname,
                           int port, const std::string &info)
    {
        return client + " " + hostname + " " + std::to_string(port) + " :" + info;
    }

    // RPL_UMODEIS (221)
    std::string rpl_umodeis(const std::string &client, const std::string &user_modes)
    {
        return client + " " + user_modes;
    }

    // RPL_LUSERSCHANNELS (254)
    std::string rpl_luserchannels(const std::string &client, int channels)
    {
        return client + " " + std::to_string(channels) + " :channels formed";
    }

    // RPL_WHOREPLY (352)
    std::string rpl_whoreply(const std::string &client, const std::string &channel,
                             const std::string &username, const std::string &host,
                             const std::string &server, const std::string &nick,
                             const std::string &flags, int hopcount,
                             const std::string &realname)
    {
        return client + " " + channel + " " + username + " " + host + " " +
               server + " " + nick + " " + flags + " " + std::to_string(hopcount) +
               " " + realname;
    }

    // RPL_ENDOFWHO (315)
    std::string rpl_endofwho(const std::string &client, const std::string &mask)
    {
        return client + " " + mask + " :End of WHO list";
    }

    // RPL_WHOISREGNICK (307)
    std::string rpl_whoisregnick(const std::string &client, const std::string &nick)
    {
        return client + " " + nick + " :has identified for this nick";
    }

    // RPL_WHOISUSER (311)
    std::string rpl_whoisuser(const std::string &client, const std::string &nick,
                              const std::string &username, const std::string &host,
                              const std::string &realname)
    {
        return client + " " + nick + " " + username + " " + host + " * :" + realname;
    }

    // RPL_WHOWASUSER (314)
    std::string rpl_whowasuser(const std::string &client, const std::string &nick,
                               const std::string &username, const std::string &host,
                               const std::string &realname)
    {
        return client + " " + nick + " " + username + " " + host + " * :" + realname;
    }

    // RPL_ENDOFWHOIS (318)
    std::string rpl_endofwhois(const std::string &client, const std::string &nick)
    {
        return client + " " + nick + " :End of /WHOIS list";
    }

    // RPL_WHOISCHANNELS (319)
    std::string rpl_whoischannels(const std::string &client, const std::string &nick,
                                  const std::vector<std::pair<char, std::string>> &channels)
    {
        std::string message = client + " " + nick + " :";
        for (unsigned int i = 0; i < channels.size(); ++i)
        {
            message += channels[i].first + channels[i].second + " ";
        }
        return message;
    }

    // RPL_LISTSTART (321)
    std::string rpl_liststart(const std::string &client)
    {
        return client + " Channel :Users Name";
    }

    // RPL_LIST (322)
    std::string rpl_list(const std::string &client, const std::string &channel, int client_count,
                         const std::string &topic)
    {
        return client + " " + channel + " " + std::to_string(client_count) + " :" + topic;
    }

    // RPL_LISTEND (323)
    std::string rpl_listend(const std::string &client)
    {
        return client + " :End of /LIST";
    }

}

#endif