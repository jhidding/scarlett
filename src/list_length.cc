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
#include "numeric/numeric.h"

using namespace Scarlett;

ptr Scarlett::length(ptr a)
{
	int i = 0;
	while (is_pair(a))
	{
		if (mark(a) == 1)
			return &infinity;
		
		set_mark(a, 1);
		a = cdr(a);
	}

	deep_mark(a, 0);
	return new Atom<int>(i);
}

