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
#include <stack>
#include <map>
#include <functional>

using namespace Scarlett;

ptr Scarlett::for_each_pair(ptr a_, std::function<void (ptr)> const &f)
{
	std::stack<ptr> A;
	A.push(a_);

	while (not A.empty())
	{
		ptr a = A.top();
		A.pop();

		if (is_pair(a))
		{
			if (mark(a) != 1)
			{
				f(a);

				set_mark(a, 1);
				A.push(car(a)); A.push(cdr(a));
			}
		}
	}

	deep_mark(a_, 0);

	return &inert;
}


ptr Scarlett::copy_es_immutable(ptr a_)
{
	std::map<ptr, ptr> AB;
	for_each_pair(a_, [&AB] (ptr a)
	{
		AB[a] = new Pair(car(a), cdr(a));
	});

	for (auto &ab : AB)
	{
		Pair *b = cast_ptr<Pair>(ab.second);
		ptr u = b->car(),
		    v = b->cdr();

		if (is_pair(u))
			b->set_car(AB[u]);

		if (is_pair(v))
			b->set_cdr(AB[v]);

		b->set_mutable(false);
	}

	return AB[a_];
}

