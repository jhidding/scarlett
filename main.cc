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
	Test::Unit::all();
	std::ofstream f("test.log", std::ios::out);
	Scarlett::Log::print_log_book(f);
	f.close();
	return 0;
}

#else

using namespace Scarlett;

int main()
{
	Environment *E = new Environment(
//	Static<Environment> E(Environment(
			list("Hello"_s, "World"_s, "Universe"_s),
			list(1_a, 2_a, 3_a), &nil);

	E->print_map(std::cout);

	God<Object>::judge();

	std::ofstream out("log.txt", std::ios::out);
	Scarlett::Log::print_log_book(out);
	out.close();
	return 0;
}

#endif

// vim:sw=4:ts=4:tw=72
