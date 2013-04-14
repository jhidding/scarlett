#include "pair.h"
#include "stack.h"

using namespace Scarlett;

bool Scarlett::pair_equiv(ptr a_, ptr b_)
{
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sa.empty())
	{
		if (sb.empty()) return false;

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_pair(a))
		{
			if (not is_pair(b)) return false;

			// we go depth first
			sa.push(cdr(a)); sb.push(cdr(b));
			sa.push(car(a)); sb.push(car(b));

			continue;
		}

		if (not is_equiv(a, b)) return false;
	}

	if (not sb.empty()) return false;

	return true;
}

