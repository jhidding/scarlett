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

#include "read.h"
#include "expression.h"
#include "../ports/ports.h"

using namespace Scarlett;

Continuation *Scarlett::read_string(Continuation *C, Environment *env, ptr args)
{
	String *s = cast_ptr<String>(car(args));
	Input_port *port = new String_input_port(s);
	return new Read::Expression(C, port);
}

#ifdef UNITTEST
#include "../unittest/test.h"
#include "../pair.h"
#include "../system/program.h"

Test::Unit Read_string_test(
	"0105 - read-string",
	"Reads some expressions from strings into Scarlett structures.",
	[] ()
{
	Static<Environment> env(&nil);
	ptr expr = list(
		"((1 2) (3 4) (5 6))\n (func 4 5 6 ...)\n 89.48@3.14\n"
		"((lambda (x . y) (+ x (car y))) 1 2 3 4 +i)\n"_a);
	Result r;
	Continuation *C = read_string(&r, &env, expr);
	while (C != &r) C = C->apply();

	//GC<Object>::cycle();
	std::cerr << deep_list_repr(r.result()) << std::endl;

	return true;
});

#endif

