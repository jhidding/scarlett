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

#include "pair.h"
#include "stack.h"
#include "misc/format.h"

using namespace Scarlett;

std::string Scarlett::Pair::repr() const
{
	return Misc::format("(", int(mark()), ": ", a->spec(), " . ", b->spec(), ")");
}

/*
std::string Scarlett::Pair::repr() const
{
	std::ostringstream ss;

	set_mark(1);
	ss << "(" << a->repr();
	ptr p = b;

	while (is_pair(p))
	{
		if (::mark(p) == 1)
		{
			ss << " ...";
			break;
		}

		::set_mark(p, 1);
		ss << " " << ::car(p)->repr();
		p = ::cdr(p);
	}

	if (is_nil(p) or is_pair(p))
		ss << ")";
	else
		ss << " . " << p->repr() << ")";

	deep_mark(b, 0);
	deep_mark(a, 0);

	return ss.str();
}
*/
