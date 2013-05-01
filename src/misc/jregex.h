/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once
#include <string>
#include <vector>
#include <regex.h>
#include <memory>

#include "wrap.h"

namespace Misc
{
	class Regex;

	class Regex_buffer
	{
		typedef const regex_t * const_ptr;
		regex_t buffer;

		public:
			Regex_buffer(std::string const &expression, int flags = REG_EXTENDED);
			~Regex_buffer();
			operator const_ptr() const;
	};

	class Regex_match
	{
		friend Regex;

		typedef std::vector<regmatch_t> regvec;
		typedef Wrap_fwd_iterator<std::string, regvec::const_iterator> 
			iterator;

		regvec match;
		std::string str;
		bool success;

		Regex_match(std::string const &s, size_t nmatch): 
			match(nmatch), str(s), success(false) {}

		regmatch_t *ref() 
			{ return match.data(); }

		iterator spawn_iterator(regvec::const_iterator const &) const;

		public:
			bool is_match() const { return success; }
			std::string operator[](size_t i) const;
			iterator begin() const;
			iterator end() const;
	};

	class Regex
	{
		std::shared_ptr<Regex_buffer> buffer;
		size_t nmatch;

		public:
			Regex() = default;

			Regex(std::string const &expression, int nsub, int flags = REG_EXTENDED):
				buffer(new Regex_buffer(expression, flags)), nmatch(nsub) {}

			Regex_match operator()(std::string const &s, int flags = 0) const;
	};
}

// vim:sw=4:ts=4:tw=72
