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

bool Scarlett::pair_equiv(ptr a_, ptr b_)
{
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sa.empty())
	{
		if (sb.empty()) return false;

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_pair(a))
		{
			if (not is_pair(b)) return false;

			// we go depth first
			sa.push(cdr(a)); sb.push(cdr(b));
			sa.push(car(a)); sb.push(car(b));

			continue;
		}

		if (not is_equiv(a, b)) return false;
	}

	if (not sb.empty()) return false;

	return true;
}

