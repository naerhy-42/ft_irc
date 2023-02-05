#include "Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const& name, Client const* creator)
			: _name(name), _topic(""), _author("")
    {
		add_client(creator);
		set_client_chanmode(creator, '+', 'o');
    }

    Channel::~Channel(void) {}

    std::string const& Channel::get_name(void) const { return _name; }

	std::string const& Channel::get_topic(void) const { return _topic; }

	std::string const& Channel::get_author(void) const { return _author; }

    std::map<Client const*, Modes> const& Channel::get_clients(void) const { return _clients; }

	Modes& Channel::get_modes_obj(void) { return _modes; }

    bool Channel::has_client(Client const* client) const { return _clients.count(client); }

	bool Channel::has_client_chanmode(Client const* client, char mode) const
	{
		std::map<Client const*, Modes>::const_iterator cit;

		for (cit = _clients.begin(); cit != _clients.end(); cit++)
		{
			if (cit->first == client)
				return cit->second.has_mode(mode);
		}
		return false;
	}

    void Channel::set_topic(std::string const& topic) { _topic = topic; }

    void Channel::set_author(std::string const& author) { _author = author; }

	void Channel::set_client_chanmode(Client const* client, char sign, char mode)
	{
		std::map<Client const*, Modes>::iterator it;

		for (it = _clients.begin(); it != _clients.end(); it++)
		{
			if (it->first == client)
				it->second.set_mode(sign, mode);
		}
	}

    void Channel::add_client(Client const* client)
	{
		_clients.insert(std::pair<Client const*, Modes>(client, Modes()));
	}

    void Channel::remove_client(Client const* client) { _clients.erase(client); }
}
