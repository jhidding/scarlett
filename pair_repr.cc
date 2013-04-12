#include "pair.h"

using namespace Scarlett;

std::string Scarlett::Pair::repr() const
{
	std::ostringstream ss;

	set_mark(1);
	ss << "(" << a->repr();
	ptr p = b;

	while (is_pair(p))
	{
		if (::mark(p) == 1)
		{
			ss << " ...";
			break;
		}

		::set_mark(p, 1);
		ss << " " << ::car(p)->repr();
		p = ::cdr(p);
	}

	if (is_nil(p) or is_pair(p))
		ss << ")";
	else
		ss << " . " << p->repr() << ")";

	deep_mark(b, 0);
	set_mark(0);

	return ss.str();
}

