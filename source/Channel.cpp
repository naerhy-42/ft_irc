#include "Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const& name, Client* creator)
			: _name(name), _topic(""), _author("")
    {
        _operators.push_back(creator);
        _clients.push_back(creator);
    }

    Channel::~Channel(void) {}

    std::string const& Channel::get_name(void) const { return _name; }

	std::string const& Channel::get_topic(void) const { return _topic; }

	std::string const& Channel::get_author(void) const { return _author; }

    std::vector<Client*> const& Channel::get_operators(void) const { return _operators; }

    std::vector<Client*> const& Channel::get_clients(void) const { return _clients; }

	Modes& Channel::get_modes_obj(void) { return _modes; }

    bool Channel::is_operator(Client const* client)
    {
        std::vector<Client*>::const_iterator cit;

        for (cit = _operators.begin(); cit != _operators.end(); cit++)
        {
            if (*cit == client)
                return true;
        }
        return false;
    }

    bool Channel::has_client(Client const* client) const
    {
        std::vector<Client*>::const_iterator cit;

        for (cit = _clients.begin(); cit != _clients.end(); cit++)
        {
            if (*cit == client)
                return true;
        }
        return false;
    }

    void Channel::set_topic(std::string const& topic) { _topic = topic; }

    void Channel::set_author(std::string const& author) { _author = author; }

    void Channel::add_client(Client* client)
    {
        if (!has_client(client))
            _clients.push_back(client);
    }

    void Channel::remove_client(Client* client)
    {
        std::vector<Client*>::iterator it;

        for (it = _clients.begin(); it != _clients.end(); it++)
        {
            if (*it == client)
            {
                _clients.erase(it);
                break;
            }
        }
    }
}
