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

Scarlett::List_metric Scarlett::c_get_list_metric(ptr l)
{
	ptr k = l;
	int p = 0, c = 0;

	while (is_pair(l) and mark(l) < 2)
	{
		switch (mark(l))
		{
			case 0: ++p;
				break;

			case 1: ++c;
				break;
		}

		set_mark(l, mark(l) + 1);
		l = cdr(l);
	}

	int n = (is_nil(l) ? 1 : 0), a = p - c;

	deep_mark(k, 0);
	return { p, n, a, c };
}

ptr Scarlett::get_list_metric(ptr l)
{
	ptr k = l;
	int p = 0, c = 0;

	while (is_pair(l) and mark(l) < 2)
	{
		switch (mark(l))
		{
			case 0: ++p;
				break;

			case 1: ++c;
				break;
		}

		set_mark(l, mark(l) + 1);
		l = cdr(l);
	}

	int n = (is_nil(l) ? 1 : 0), a = p - c;

	deep_mark(k, 0);
	return list(
		new Atom<int>(p), new Atom<int>(n),
		new Atom<int>(a), new Atom<int>(c));
}

