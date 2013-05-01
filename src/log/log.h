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

#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <memory>

namespace Scarlett {
	extern std::ostream &operator<<(std::ostream &out,
			std::pair<double, std::string> const &E);

	class Log
	{
		typedef std::pair<double, std::string> Entry;

		static std::unique_ptr<std::set<Log *>> _log_book;
		static std::set<Log *> &log_book()
		{
			if (not _log_book)
				_log_book = std::unique_ptr<std::set<Log *>>(new std::set<Log *>);
			return *_log_book;
		}

		std::vector<Entry> 	entries;
		std::string 		prefix;
		bool				echo;

		private:
			static void format(std::ostream &out) {}

			template <typename First, typename ...Rest>
			static void format(std::ostream &out, First a, Rest &&...rest)
			{
				out << a;
				format(out, std::forward<Rest>(rest)...);
			}

		public:
			static void print_log_book(std::ostream &out);

			Log(std::string const &prefix_, bool echo_ = false);
			~Log();

			void time_stamp(std::string const &tag = "");

			template <typename ...Args> void entry(Args &&...args);
	};
}

#include "log.hh"

// vim:sw=4:ts=4:tw=72
