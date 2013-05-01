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

#include <memory>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>

//#include "system.h"
#include "system/environment.h"

#ifdef UNITTEST

#include "unittest/test.h"
#include "log/log.h"

int main()
{
	Scarlett::Test::Unit::all();
	std::ofstream f("test.log", std::ios::out);
	Scarlett::Log::print_log_book(f);
	f.close();
	return 0;
}

#else

#include "read/read.h"
#include "system/system.h"

using namespace Scarlett;

int main()
{
	Static<Environment> env(&nil);
	load_global_env(env);

	Result r;
	Program p(apply(&r, &env, &Eval, list(cons(&Sequence, read_istream(std::cin)))), &r);
	p.run();

	std::cout << repr(r.result()) << std::endl;
	return 0;
}

#endif

// vim:sw=4:ts=4:tw=72
