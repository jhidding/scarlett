#include "read.H"
#include "expression.h"
#include "../ports/ports.h"
#include "../system/system.h"

using namespace Scarlett;

Continuation *Scarlett::read_input_port(Continuation *C, Environment *env, ptr args)
{
	Input_port *port = cast_ptr<Input_port>(car(args));
	return new Read::Expression(C, port);
}

ptr Scarlett::read_istream(std::istream &in_)
{
	Static<Environment> env(&nil);
	Result r;

	Stream_input_port *port = new Stream_input_port(in_);
	Program(read_input_port(&r, &env, list(port)), &r).run();
	return r.result();
}

