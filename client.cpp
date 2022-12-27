#include "client.hpp"

namespace ft
{
	client::client(void) {}

	std::string const& client::get_nickname(void) const { return _nickname; }

	void client::set_nickname(std::string const& nickname) { _nickname = nickname; }
}
