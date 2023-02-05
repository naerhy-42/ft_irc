#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Modes.hpp"

namespace ft
{
    class Channel
    {
    public:
        Channel(std::string const &name, Client* creator);
        ~Channel(void);

        std::string const& get_name(void) const;
        std::string const& get_topic(void) const;
        std::string const& get_author(void) const;
		std::vector<Client*> const& get_operators(void) const;
        std::vector<Client*> const& get_clients(void) const;
		Modes& get_modes_obj(void);

        bool is_operator(Client const* client);

        bool has_client(Client const* client) const;

        void set_topic(std::string const& topic);
        void set_author(std::string const& author);

        void add_client(Client* client);
        void remove_client(Client* client);

    private:
		// Channel(Channel const& x);
		// Channel& operator=(Channel const& x);

        std::string _name;
        std::string _topic;
        std::string _author;
        std::vector<Client*> _operators;
        std::vector<Client*> _clients;
		Modes _modes;
    };
}

#endif
