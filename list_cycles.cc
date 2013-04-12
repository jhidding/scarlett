#include "pair.h"
#include "stack.h"

using namespace Scarlett;

ptr Scarlett::deep_mark(ptr a, char m)
{
	Static<Stack> sa;
	sa.push(a);

	while (not sa.empty())
	{
		a = sa.top();
		sa.pop();

		if (is_pair(a))
		{
			Pair *aa = cast_ptr<Pair>(a);

			if (aa->mark() != m)
			{
				aa->set_mark(m);
				sa.push(aa->car());
				sa.push(aa->cdr());
			}
		}
	}

	return &inert;
}

bool Scarlett::deep_compare(ptr a_, ptr b_, char m)
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

			if (mark(a) != m or mark(b) != m)
			{
				set_mark(a, m); set_mark(b, m);
				// we go depth first
				sa.push(cdr(a)); sb.push(cdr(b));
				sa.push(car(a)); sb.push(car(b));
			}

			continue;
		}

		if (not is_equiv(a, b)) return false;
	}

	if (not sb.empty()) return false;

	return true;
}

bool Scarlett::deep_congruence(ptr a_, ptr b_, char m)
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

			if (mark(a) != m or mark(b) != m)
			{
				set_mark(a, m); set_mark(b, m);
				// we go depth first
				sa.push(cdr(a)); sb.push(cdr(b));
				sa.push(car(a)); sb.push(car(b));
			}

			continue;
		}
	}

	if (not sb.empty()) return false;

	return true;
}

