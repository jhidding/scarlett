#pragma once

#include <string>
#include <sstream>
#include <utility>

namespace Misc
{
	inline void format_to(std::ostream &out) {}

	template <typename First, typename ...Rest>
	void format_to(std::ostream &out, First a, Rest &&...rest)
	{
		out << a;
		format_to(out, std::forward<Rest>(rest)...);
	}

	template <typename ...Args>
	std::string format(Args &&...args)
	{
		std::ostringstream ss;
		format_to(ss, std::forward<Args>(args)...);
		return ss.str();
	}
}

