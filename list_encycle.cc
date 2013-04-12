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

