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
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		extern ptr string_to_number(std::string const &s);

		class Number: public Reader
		{
			std::string s;

			public:
				Number(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("number: `", s, "'"); }

				virtual Continuation *put(int ch)
				{
					if (isspace(ch) or ch == ')' or ch == '(' or ch == '[' or ch == ']')
						return cast_ptr<Reader>(parent()->supply(string_to_number(s)))->put(ch);

					s += ch;
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
