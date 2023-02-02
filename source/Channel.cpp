#include "Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const& name, Client* creator)
			: _name(name), _topic(""), _author("")
    {
        _operators.push_back(creator);
        _clients.push_back(creator);
    }

    Channel::~Channel() {}

    std::string const& Channel::get_name(void) const { return _name; }

	std::string const& Channel::get_topic(void) const { return _topic; }

	std::string const& Channel::get_author(void) const { return _author; }

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

    std::vector<Client*> const& Channel::get_operators(void) const { return _operators; }

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

    std::vector<Client*> const& Channel::get_clients(void) const { return _clients; }

	bool Channel::has_mode(char mode) const
	{
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
		{
			if (*cit == mode)
				return true;
		}
		return false;
	}

	std::vector<char> const& Channel::get_modes(void) const { return _modes; }

	std::string Channel::get_modes_str(void) const
	{
		std::string modes_str;
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
			modes_str.append(1, *cit);
		return modes_str;
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

	void Channel::set_mode(char sign, char mode)
	{
		if (sign == '+')
			_modes.push_back(mode);
		else
		{
			std::vector<char>::iterator it;

			for (it = _modes.begin(); it != _modes.end(); it++)
			{
				if (*it == mode)
				{
					_modes.erase(it);
					break;
				}
			}
		}
	}
}
