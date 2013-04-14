#include "pair.h"

using namespace Scarlett;

ptr Scarlett::transpose(ptr a)
{
	
}

ptr Scarlett::unzip(ptr a)
{
	ptr b = &nil, c = &nil;

	for (ptr i : List(a))
	{
		b = cons(car(i), b);
		c = cons(cadr(i), c);
	}

	return list(b, c);
}

