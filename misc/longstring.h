#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <functional>

namespace Misc
{
	template <typename I>
	void split(std::string const &s, char d, I inserter)
	{
		size_t p, q = 0;
		while (q < s.length())
		{
			p = s.find_first_not_of(d, q);
			if (p == std::string::npos) return;

			q = s.find_first_of(d, p);
			if (q == std::string::npos)
			{
				*inserter = s.substr(p, s.length() - p);
				return;
			}
			else
			{
				*inserter = s.substr(p, q-p);
				++inserter;
			}
		}
	}

	class LongString
	{
		std::vector<std::string> words;
		unsigned width;
		std::function<std::string ()> left;

		public:
			LongString(std::string const &s, unsigned width_,
				std::function<std::string ()> const &left_):
				width(width_), left(left_)
			{
				split(s, ' ', std::back_inserter(words));
			}

			std::ostream &print(std::ostream &out) const
			{
				std::ostringstream ss(left(), std::ios::ate);
				auto i = words.begin();
				while (i != words.end())
				{
					while (i != words.end())
					{
						if (ss.str().length() + i->length() > width)
							break;

						ss << *i << " ";
						++i;
					}

					out << ss.str() << std::endl;
					ss.str(left());
				}

				return out;
			}
	};

	inline std::ostream &operator<<(std::ostream &out, LongString const &ls)
	{
		return ls.print(out);
	}
}

