#ifndef __MODES_HPP__
#define __MODES_HPP__

#include <string>
#include <vector>

namespace ft
{
	class Modes
	{
		public:
			Modes(void);
			Modes(Modes const& x);
			Modes& operator=(Modes const& x);
			~Modes(void);

			std::vector<char> const& get_modes(void) const;
			std::string get_modes_str(void) const;

			bool has_mode(char mode) const;

			void set_mode(char sign, char mode);

		private:
			std::vector<char> _modes;
	};
}

#endif
