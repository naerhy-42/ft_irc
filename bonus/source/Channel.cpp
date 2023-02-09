#include "Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const& name, Client const* creator) : _name(name)
    {
		get_modes_obj().set_mode('+', 't');
		add_client(creator);
		set_client_chanmode(creator, '+', 'o');
    }

	Channel::Channel(Channel const& x)
			: _name(x._name), _topic(x._topic), _clients(x._clients), _modes(x._modes) {}

	Channel& Channel::operator=(Channel const& x)
	{
		if (this != &x)
		{
			_name = x._name;
			_topic = x._topic;
			_clients = x._clients;
			_modes = x._modes;
		}
		return *this;
	}

    Channel::~Channel(void) {}

    std::string const& Channel::get_name(void) const { return _name; }

	std::string const& Channel::get_topic(void) const { return _topic; }

	std::string Channel::get_client_prefix(Client const* client) const
	{
		if (has_client_chanmode(client, 'o'))
			return "@";
		else if (has_client_chanmode(client, 'v'))
			return "+";
		else
			return "";
	}

    std::map<Client const*, Modes> const& Channel::get_clients(void) const { return _clients; }

	Modes& Channel::get_modes_obj(void) { return _modes; }
	Modes const& Channel::get_modes_obj(void) const { return _modes; }

	bool Channel::is_moderated(void) const { return get_modes_obj().has_mode('m'); }

	bool Channel::is_topic_restricted(void) const { return get_modes_obj().has_mode('t'); }

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

	bool Channel::has_talk_privilege(Client const* client) const
	{
		if (has_client_chanmode(client, 'v') || has_client_chanmode(client, 'o')
				|| client->is_global_operator())
			return true;
		return false;
	}

    void Channel::set_topic(std::string const& topic) { _topic = topic; }

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
