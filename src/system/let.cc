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

	Environment *n = new Environment(car(defs), cadr(defs), list(env));
	return apply(C, n, &Eval, list(cons(&Sequence, body)));
}

Global<C_operative> Scarlett::Let("$let", internal_let);

