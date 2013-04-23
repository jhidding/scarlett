#include <cstdlib>
#include "read.H"

using namespace Scarlett;
using namespace Read;

Continuation *Scarlett::Read::Reader::apply()
{
	int ch = in->get();

	if (ch == '\n') ++line_num;

	try
	{
		return put(ch);
	}
	catch (Exception const &e)
	{
		std::cerr << "Syntax error on line " << line_num << ":\n";
		std::cerr << e << std::endl;
		exit(-1);
	}
}

