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

#include "../object.h"
#include "system.h"
#include "../pair.h"
#include "../read/read.h"
#include "safety.h"

using namespace Scarlett;

#ifdef UNITTEST
#include "../unittest/test.h"
#include "environment.h"
#include "program.h"

Test::Unit Internal_let_test(
	"0202 - $let",
	"$let is a slightly higher order operative. I test the arguments "
	"for sanity using the cyclic list congruence test. Let's see if "
	"it works!",
	[] ()
{
	Static<Environment> env(&nil);
	load_global_env(env);

	Result r;
	Program(apply(&r, &env, &Eval, "($let ((a 1) (b 2)) (+ a b))"_e), &r).run();
	std::cerr << r.result()->repr() << std::endl;

	return true;
});

#endif

Continuation *internal_let(Continuation *C, Environment *env, ptr args)
{
	assert_that("$let", args, Congruent_with("(((<arg1> <expr1>) ...) <body> ...)"_e));
	ptr defs = unzip(car(args));
	ptr body = cdr(args);

	
	//Environment *n = new Environment(car(defs), cadr(defs), list(env));
	return apply(C, env, &Eval, list(cons(list(&Lambda, car(defs), 
		cons(&Sequence, body)), cadr(defs))));
}

Global<C_operative> Scarlett::Let("$let", internal_let);

