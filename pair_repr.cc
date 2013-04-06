#include "pair.h"

using namespace Scarlett;

std::string Scarlett::Pair::repr() const
{
	std::ostringstream ss;

	ss << "(" << a->repr();
	ptr p = b;

	while (is_pair(p))
	{
		ss << " " << ::car(p)->repr();
		p = ::cdr(p);
	}

	if (is_nil(p))
		ss << ")";
	else
		ss << " . " << p->repr() << ")";

	return ss.str();
}

