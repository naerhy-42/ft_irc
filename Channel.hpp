#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "Client.hpp"
#include <string>
#include <vector>


namespace ft
{
    class Channel
    {
        public:
            Channel(std::string const& name);
            ~Channel();

            std::string const& get_name(void) const;
            std::vector<Client> const& get_clients(void) const;

            void add_client(Client const& client);
            void remove_client(Client const& client);
            bool has_client(Client const& client) const;

        private:
            std::string _name;
            std::vector<Client> _clients;
    };
}


#endif