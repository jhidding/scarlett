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

