#include "pair.h"
#include "numeric/numeric.h"

using namespace Scarlett;

ptr Scarlett::length(ptr a)
{
	int i = 0;
	while (is_pair(a))
	{
		if (mark(a) == 1)
			return &infinity;
		
		set_mark(a, 1);
		a = cdr(a);
	}

	deep_mark(a, 0);
	return new Atom<int>(i);
}

