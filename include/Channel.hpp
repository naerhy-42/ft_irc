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
        Channel(std::string const &name, Client* creator);
        ~Channel();

        std::string const &get_name(void) const;
        std::vector<Client*> const& get_clients(void) const;
        int get_size() const;
		std::vector<char> const& get_modes(void) const;
		std::string get_modes_str(void) const;

        std::string get_topic(void) const;
        std::string get_author(void) const;
        void set_topic(std::string topic);
        void set_author(std::string author);


        void add_client(Client* client);
        void remove_client(Client* client);
        bool has_client(Client* client) const;
		void set_mode(char sign, char mode);
        
        bool is_operator(Client* client);

    private:
        std::string _name;
        std::string _topic;
        std::string _author;
        std::vector<Client*> _operators;
        std::vector<Client*> _clients;
        size_t _size;
		std::vector<char> _modes;
    };
}

#endif
