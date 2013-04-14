#include <ctime>
#include <sstream>
#include <algorithm>

namespace Scarlett 
{
	template <typename ...Args>
	void Log::entry(Args &&...args)
	{

		std::ostringstream s;
		format(s, prefix, ": ", std::forward<Args>(args)...);
		timespec t;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);
		double sec = t.tv_sec + t.tv_nsec / 1e9;

		entries.push_back(Entry(sec, s.str()));

		if (echo)
		{
			std::cerr << entries.back() << std::endl;
		}
	}
}

