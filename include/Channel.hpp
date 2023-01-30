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
        Channel(std::string const &name, ft::Client creator);
        ~Channel();

        std::string const &get_name(void) const;
        std::vector<Client> const &get_clients(void) const;
        int get_size() const;

        std::string get_topic(void) const;
        std::string get_author(void) const;
        void set_topic(std::string topic);
        void set_author(std::string author);


        void add_client(Client const &client);
        void remove_client(Client const &client);
        bool has_client(Client const &client) const;
        
        bool is_operator(Client const &client);

    private:
        std::string _name;
        std::string _topic;
        std::string _author;
        std::vector<Client> _operators;
        std::vector<Client> _clients;
        size_t _size;
    };
}

#endif
