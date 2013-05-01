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

ptr Scarlett::list_tail(ptr a, int i)
{
	for (int j = 0; j < i; ++j)
	{
		a = cdr(a);
	}

	return a;
}

ptr Scarlett::encycle(ptr a, int i, int j)
{
	if (j > 0)
		set_cdr(list_tail(a, i + j - 1), list_tail(a, i));

	return &inert;
}

