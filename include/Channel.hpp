#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "Client.hpp"
#include <string>
#include <vector>
#include <iostream>

namespace ft
{
    class Channel
    {
    public:
        Channel(std::string const &name, std::string const &creator);
        ~Channel();

        std::string const &get_name(void) const;
        std::vector<Client> const &get_clients(void) const;
        int get_size() const;
		std::vector<char> const& get_modes(void) const;
		std::string get_modes_str(void) const;


        void add_client(Client const &client);
        void remove_client(Client const &client);
        bool has_client(Client const &client) const;
		bool is_operator(std::string const& nickname) const;
		void set_mode(char sign, char mode);

    private:
        std::string _name;
        std::vector<std::string> _operators;
        std::vector<Client> _clients;
        size_t _size;
		std::vector<char> _modes;
    };
}

#endif
