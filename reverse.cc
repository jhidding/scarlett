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

