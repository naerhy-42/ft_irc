#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <iostream>
#include <map>
#include <string>

#include "Client.hpp"
#include "Modes.hpp"

namespace ft
{
    class Channel
    {
    public:
        Channel(std::string const &name, Client const* creator);
        ~Channel(void);

        std::string const& get_name(void) const;
        std::string const& get_topic(void) const;
        std::string const& get_author(void) const;
        std::map<Client const*, Modes> const& get_clients(void) const;
		Modes& get_modes_obj(void);

        bool has_client(Client const* client) const;
		bool has_client_chanmode(Client const* client, char mode) const;

        void set_topic(std::string const& topic);
        void set_author(std::string const& author);
		void set_client_chanmode(Client const* client, char sign, char mode);

        void add_client(Client const* client);
        void remove_client(Client const* client);

    private:
		// Channel(Channel const& x);
		// Channel& operator=(Channel const& x);

        std::string _name;
        std::string _topic;
        std::string _author;
		std::map<Client const*, Modes> _clients;
		Modes _modes;
    };
}

#endif
