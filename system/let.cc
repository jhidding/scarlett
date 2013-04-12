#include "system.h"
#include "../pair.h"

using namespace Scarlett;

Continuation *internal_let(Continuation *C, Environment *env, ptr args)
{
	//assert_that("$let", args, Congruent_with("(((<arg1> <expr1>) ...) <body> ...)"));
	ptr defs = unzip(car(args));
	ptr body = cdr(args);

	Environment *n = new Environment(car(defs), cadr(defs), list(env));
	return apply(C, n, &Eval, list(cons(&Sequence, body)));
}

Global<C_operative> Scarlett::Let("$let", internal_let);

