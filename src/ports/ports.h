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
#include "../object.h"
#include "../string.h"
#include <iostream>
#include <sstream>
#include <string>

namespace Scarlett
{
	class Input_port: public Object
	{
		public:
			virtual std::string type_name() const { return "input-port"; }
			virtual bool is_mutable() const { return true; }
			virtual int get() = 0;
	};

	class Stream_input_port: public Input_port
	{
		std::istream in;

		public:
			Stream_input_port(std::istream &in_):
				in(in_.rdbuf()) {}

			int get() { return in.get(); }
	};

	class String_input_port: public Input_port
	{
		std::istringstream in;

		public:
			String_input_port(std::string const &s):
				in(s) {}

			String_input_port(String *s):
				in(s->value()) {}

			int get() { return in.get(); }
	};
}

