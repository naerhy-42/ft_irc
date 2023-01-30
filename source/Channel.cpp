#include "../include/Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const &name, Client* creator) : _name(name), _topic(""), _author(""), _size(0)
    {
        _operators.push_back(creator);
        std::cout << "CHANNEL CREATED" << std::endl;
    }

    Channel::~Channel() { std::cout << "CHANNEL DELETED" << std::endl; }

    std::string const &Channel::get_name(void) const
    {
        return _name;
    }

    std::vector<Client*> const &Channel::get_clients(void) const
    {
        return _clients;
    }

    int Channel::get_size() const { return _size; }

    void Channel::add_client(Client* client)
    {
        if (!has_client(client))
        {
            _clients.push_back(client);
            ++_size;
        }
    }

    void Channel::remove_client(Client* client)
    {
        std::vector<Client*>::iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (*it == client)
            {
                _clients.erase(it);
                --_size;
                break;
            }
        }
    }

    bool Channel::has_client(Client* client) const
    {
        std::vector<Client*>::const_iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (*it == client)
            {
                std::cout << "current channel size : " << _size << std::endl;
                return true;
            }
        }
        return false;
    }

	std::vector<char> const& Channel::get_modes(void) const { return _modes; }

	std::string Channel::get_modes_str(void) const
	{
		std::string ret;

		for (size_t i = 0; i < _modes.size(); i++)
			ret.append(1, _modes[i]);
		return ret;
	}

	void Channel::set_mode(char sign, char mode)
	{
		if (sign == '+')
			_modes.push_back(mode);
		else
		{
			for (std::vector<char>::iterator it = _modes.begin(); it != _modes.end(); it++)
			{
				if (*it == mode)
				{
					_modes.erase(it);
					break;
				}
			}
		}
	}

    bool Channel::is_operator(Client* client)
    {
        std::vector<Client*>::const_iterator it;
        for (it = _operators.begin(); it != _operators.end(); ++it)
        {
            if (*it == client)
            {
                // std::cout << "current channel size : " << _size << std::endl;
                return true;
            }
        }
        return false;
    }

    std::string Channel::get_topic(void) const { return _topic;}
    std::string Channel::get_author(void) const { return _author;}


    void Channel::set_topic(std::string topic)
    {
        _topic = topic;
    }
    void Channel::set_author(std::string author)
    {
        _author = author;
    }
}
