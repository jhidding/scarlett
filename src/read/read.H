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

#include "../system/system.h"
#include "../ports/ports.h"
#include "../misc/format.h"

#include <ctype.h>

namespace Scarlett
{
	namespace Read
	{
		enum Msg { Improper, Circular };

		class Reader: public Continuation
		{
			Input_port 	*in;
			size_t		line_num;

			public:
				Reader(Continuation *parent, Input_port *in_):
					Continuation(parent), in(in_),
		       			line_num(0)
				{}

				Reader(Continuation *parent):
					Continuation(parent), in(cast_ptr<Reader>(parent)->in) {}

				std::string state() const { return "reader"; }

				virtual void gc(Edict const &cmd) const
					{ Continuation::gc(cmd); cmd(in); }

				virtual void poke(Msg a) {}

				virtual Continuation *put(int ch) = 0;

				virtual Continuation *supply(ptr a)
					{ return parent(); }

				virtual Continuation *apply();
		};
	}
}

