#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <sstream>
#include <string>
#include <vector>

#include <iostream>

namespace ft
{
	class message
	{
		public:
			message(std::string const& base_message);
			void split(void);
			void print(void) const;

			std::string const& get_origin(void) const;
			std::string const& get_command(void) const;
			std::vector<std::string> const& get_parameters(void) const;
			std::string const& get_remainder(void) const;

		private:
			static int const _max_chars;
			static int const _max_params;

			std::string _base_message;
			std::string _origin;
			std::string _command;
			std::vector<std::string> _parameters;
			std::string _remainder;
	};
}

#endif
