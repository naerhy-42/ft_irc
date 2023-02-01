#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <iostream>
#include <string>
#include <vector>

#include "Client.hpp"

namespace ft
{
    class Channel
    {
    public:
        Channel(std::string const &name, Client* creator);
        ~Channel();

        std::string const& get_name(void) const;
        std::string const& get_topic(void) const;
        std::string const& get_author(void) const;

        bool is_operator(Client const* client);
		std::vector<Client*> const& get_operators(void) const;
        bool has_client(Client const* client) const;
        std::vector<Client*> const& get_clients(void) const;

		bool has_mode(char mode) const;
		std::vector<char> const& get_modes(void) const;
		std::string get_modes_str(void) const;

        void set_topic(std::string const& topic);
        void set_author(std::string const& author);

        void add_client(Client* client);
        void remove_client(Client* client);

		void set_mode(char sign, char mode);

    private:
		// Channel(Channel const& x);
		// Channel& operator=(Channel const& x);

        std::string const _name;
        std::string _topic;
        std::string _author;
        std::vector<Client*> _operators;
        std::vector<Client*> _clients;
		std::vector<char> _modes;
    };
}

#endif
