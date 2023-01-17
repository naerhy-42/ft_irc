#include "Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const &name, std::string const &creator) : _name(name)
    {
        _operators.push_back(creator);
        std::cout << "CHANNEL CREATED" << std::endl;
    }

    Channel::~Channel() { std::cout << "CHANNEL DELETED" << std::endl; }

    std::string const &Channel::get_name(void) const
    {
        return _name;
    }

    std::vector<Client> const &Channel::get_clients(void) const
    {
        return _clients;
    }

    void Channel::add_client(Client const &client)
    {
        if (!has_client(client))
        {
            _clients.push_back(client);
        }
    }

    void Channel::remove_client(Client const &client)
    {
        std::vector<Client>::iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (*it == client)
            {
                _clients.erase(it);
                break;
            }
        }
    }

    bool Channel::has_client(Client const &client) const
    {
        std::vector<Client>::const_iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (*it == client)
            {
                return true;
            }
        }
        return false;
    }
}
