#include "pair.h"
#include "stack.h"

using namespace Scarlett;

void Scarlett::match_tree(ptr a_, ptr b_, std::function<void (ptr, ptr)> const &f)
{
//	std::cerr << "matching: " << repr(a_) << " -> " << repr(b_) << std::endl;;
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sa.empty())
	{
		if (sb.empty())
			throw Exception(ERROR_match, "not enough arguments");

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_nil(a))
		{
			if (not is_nil(b)) throw Exception(ERROR_match, "a is nil, but b isn't",
					a->repr(), " ", b->repr());
			continue;
		}

		if (is_ignore(a))
		{
			continue;
		}

		if (is_pair(a))
		{
			if (not is_pair(b)) throw Exception(ERROR_match, "a is pair, but b isn't",
					a->repr(), " ", b->repr());

			sa.push(cdr(a)); sa.push(car(a));
			sb.push(cdr(b)); sb.push(car(b));

			continue;
		}

		if (is_symbol(a))
		{
			f(a, b);

			continue;
		}

		throw Exception(ERROR_match, "not a pair, nil or symbol",
				a->repr(), " ", b->repr());
	}
}

// vim:sw=4:ts=4:tw=72
