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

using namespace Scarlett;

void Scarlett::match_tree(ptr a_, ptr b_, std::function<void (ptr, ptr)> const &f)
{
//	std::cerr << "matching: " << repr(a_) << " -> " << repr(b_) << std::endl;;
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sa.empty())
	{
		if (sb.empty())
			throw Exception(ERROR_match, "not enough arguments");

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_nil(a))
		{
			if (not is_nil(b)) throw Exception(ERROR_match, "a is nil, but b isn't",
					repr(a_), " <-> ", repr(b_));
			continue;
		}

		if (is_ignore(a))
		{
			continue;
		}

		if (is_pair(a))
		{
			if (not is_pair(b)) throw Exception(ERROR_match, "a is pair, but b isn't",
					repr(a_), " <-> ", repr(b_));

			sa.push(cdr(a)); sa.push(car(a));
			sb.push(cdr(b)); sb.push(car(b));

			continue;
		}

		if (is_symbol(a))
		{
			f(a, b);

			continue;
		}

		throw Exception(ERROR_match, "not a pair, nil or symbol",
				a->repr(), " ", b->repr());
	}
}

// vim:sw=4:ts=4:tw=72
