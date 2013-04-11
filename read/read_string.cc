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

Test::Unit Read_string_test(
	"0105 - read-string",
	"Reads some expressions from strings into Scarlett structures.",
	[] ()
{
	Static<Environment> env(&nil);
	ptr expr = list(
		"(func 4 5 6)\n 89.48@3.14\n ((lambda (x . y) (+ x (car y))) 1 2 3 4 +i)\n"_a);
	Test::Result r;
	Test::Program program(read_string(&r, &env, expr), &r);

	for (auto &cc : program) 
	{
//		std::cerr << cc.state() << std::endl;
	}

	GC<Object>::cycle();
	std::cerr << r.result()->repr() << std::endl;

	return true;
});

#endif

