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

