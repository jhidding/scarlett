#include "string.h"

using namespace Scarlett;

std::map<char, std::string> Scarlett::String::char_repr = {
	{'\n', "\\n"}, {'\r', "\\r"}, {'\t', "\\t"}, {'\033', "\\e"}, {'\b', "\\b"}};

#ifdef UNITTEST
#include "unittest/test.h"

Test::Unit String_test(
	"0020 - strings",
	"Testing functionality of strings.",
	[] ()
{
	String *s = "Hello World!\n"_a;
	std::cerr << s->value();

	if (s->repr() != "\"Hello World!\\n\"")
		throw Exception(ERROR_fail, "the external representation of the test-string is not correct.");

	return true;
});

#endif

