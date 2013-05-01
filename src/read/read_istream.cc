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
	//Program(read_input_port(&r, &env, list(port)), &r).run();
	Continuation *C = read_input_port(&r, &env, list(port));
	while (C != &r) C = C->apply();
	return r.result();
}

