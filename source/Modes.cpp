#include "Modes.hpp"

namespace ft
{
	Modes::Modes(void) {}

	Modes::Modes(Modes const& x) : _modes(x._modes) {}

	Modes& Modes::operator=(Modes const& x)
	{
		if (this != &x)
			_modes = x._modes;
		return *this;
	}

	Modes::~Modes(void) {}

	std::vector<char> const& Modes::get_modes(void) const { return _modes; }

	std::string Modes::get_modes_str(void) const
	{
		std::string modes_str;
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
			modes_str.append(1, *cit);
		return modes_str;
	}

	bool Modes::has_mode(char mode) const
	{
		std::vector<char>::const_iterator cit;

		for (cit = _modes.begin(); cit != _modes.end(); cit++)
		{
			if (*cit == mode)
				return true;
		}
		return false;
	}

	void Modes::set_mode(char sign, char mode)
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
