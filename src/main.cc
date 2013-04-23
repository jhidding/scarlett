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
