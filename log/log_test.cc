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

