#pragma once

#include <iostream>
#include <exception>

namespace Scarlett
{
	enum ErrNo { ERROR, ERROR_memory, ERROR_bound, ERROR_unbound, ERROR_match, ERROR_fail, ERROR_syntax };

	class Exception: public std::exception
	{
		ErrNo 		id;
		std::string descr;

		static void format(std::ostream &out) {}

		template <typename First, typename ...Rest>
		static void format(std::ostream &out, First a, Rest &&...rest)
		{
			out << a;
			format(out, std::forward<Rest>(rest)...);
		}

		public:
			template <typename ...Args>
			Exception(ErrNo id_, Args &&...args):
				id(id_)
			{
			 	std::stringstream s;	
				format(s, std::forward<Args>(args)...);
				descr = s.str();
			}

			bool is(ErrNo x) const { return x == id; }
			std::string const &description() const { return descr; }

			char const *what() const throw() { return descr.c_str(); }
	};

	inline std::ostream &operator<<(std::ostream &out, Exception const &E)
	{
		out << E.description();
		return out;
	}
}

// vim:sw=4:ts=4:tw=72
