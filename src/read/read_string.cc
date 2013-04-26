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

