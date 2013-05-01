/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <iostream>
#include <exception>

namespace Scarlett
{
	enum ErrNo { ERROR, ERROR_memory, ERROR_bound, ERROR_unbound, ERROR_match, ERROR_fail, ERROR_syntax, ERROR_ni };

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
