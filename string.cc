#include "string.h"
#include <iomanip>
#include <sstream>

using namespace Scarlett;

std::map<char, std::string> Scarlett::String::char_repr = {
	{'\n', "\\n"}, {'\r', "\\r"}, {'\t', "\\t"}, {'\033', "\\e"}, {'\b', "\\b"}};

std::string Scarlett::String::repr() const
{
	std::ostringstream ss;
	ss << '"';

	for (char p : value())
	{
		if (p < 32)
			if (char_repr.count(p) == 1)
				ss << char_repr[p];
			else
				ss << "\\x" << std::hex 
				   << std::setw(2) << std::setfill('0')
				   << unsigned(p);

		else
			ss << p;
	}
	ss << '"';
	return ss.str();
}

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

