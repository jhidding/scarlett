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

#include "log.h"

#ifdef UNITTEST
#include "../unittest/test.h"
#include <sstream>

using namespace Scarlett;

Test::Unit Log_unittest(
		"0000 - log",
		"Should create two logs accepting some entries. "
		"The output to std::cerr and the ostringstream should "
		"be identical. ", [] ()
{
	Log A("prefix 1", false);
	A.entry("first item. ", 9.0);
	Log B("prefix 2", false);
	B.entry("second item. ", 11.0);
	A.entry("third item. ", 13.0);

/*	std::ostringstream ss;
	Log::print_log_book(ss);
	std::cout << ss.str();
*/
	return true;
});

#endif

