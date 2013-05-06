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
