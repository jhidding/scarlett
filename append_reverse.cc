#include "pair.h"

using namespace Scarlett;

ptr append_reverse(ptr a, ptr b)
{
	while (not is_nil(a))
	{
		b = cons(car(a), b);
		a = cdr(a);
	}

	return b;
}

