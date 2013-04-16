#include "system.h"
#include "closure.h"
#include "safety.h"

using namespace Scarlett;

Continuation *internal_vau(Continuation *C, Environment *env, ptr args)
{
	assert_that("vau", args, Is_proper_list());

	ptr tree 	= car(args);
	ptr dyn_env_sym = cadr(args);
	ptr body 	= cddr(args);

	return C->supply(new Closure(env, dyn_env_sym, tree, copy_es_immutable(body)));
}

Continuation *internal_lambda(Continuation *C, Environment *env, ptr args)
{
	assert_that("lambda", args, Is_proper_list());

	ptr tree = car(args);
	ptr body = cdr(args);

	return C->supply((new Closure(env, &ignore, tree, copy_es_immutable(body)))->wrap());
}

Global<C_operative> Scarlett::Vau("$vau", internal_vau);
Global<C_operative> Scarlett::Lambda("$lambda", internal_lambda);

