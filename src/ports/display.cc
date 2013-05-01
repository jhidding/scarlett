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

#include "../system/system.h"
#include <iostream>

using namespace Scarlett;

Continuation *internal_display(Continuation *C, Environment *env, ptr args)
{
	assert_that("display", args, Is_proper_list());
	ptr tmp = args;

	while (is_pair(args))
	{
		if (mark(args) == 1) break;
		set_mark(args, 1);

		if (is_pair(car(args)))
			std::cout << deep_list_repr(car(args));
		else
			std::cout << car(args)->str();

		args = cdr(args);
	}

	deep_mark(tmp, 0);
	return C->supply(&inert);
}

Global<C_applicative> Display("display", internal_display);

