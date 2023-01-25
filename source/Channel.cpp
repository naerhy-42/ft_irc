#include "../include/Channel.hpp"

namespace ft
{
    Channel::Channel(std::string const &name, std::string const &creator) : _name(name),  _size(0)
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


    int Channel::get_size() const { return _size; }

    void Channel::add_client(Client const &client)
    {
        if (!has_client(client))
        {
            _clients.push_back(client);
            ++_size; 
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
                --_size;
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

	bool Channel::is_operator(std::string const& nickname) const
	{
		for (size_t i = 0; i < _operators.size(); i++)
		{
			if (nickname == _operators[i])
				return true;
		}
		return false;
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
}
