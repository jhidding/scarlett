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
using namespace Scarlett;

ptr Scarlett::reverse(ptr a)
{
	ptr p = &nil;

	for (ptr h : List(a))
	{
		p = cons(h, p);
	}

	return p;
}

ptr Scarlett::improper_reverse(ptr a, ptr b)
{
	while (is_pair(a))
	{
		b = cons(car(a), b);
		a = cdr(a);
	}

	return b;
}

