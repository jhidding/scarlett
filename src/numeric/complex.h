#pragma once
#include "../pair.h"

namespace Scarlett
{
	inline ptr make_rectangular(ptr a, ptr b)
	{
		return list("stub:make-rectangular"_s, a, b);
	}

	inline ptr make_polar(ptr a, ptr b)
	{
		return list("stub:make-polar"_s, a, b);
	}
}

