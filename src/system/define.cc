#include "system.h"
#include "safety.h"
#include "continuation.h"
#include "combiners.h"

using namespace Scarlett;

Continuation *internal_define(Continuation *C, Environment *env, ptr args)
{
	assert_that("$define!", args, Has_proper_length(2));

	ptr A = car(args);
	Continuation *nc = new Apply(C, env, new C_closure(
		[A] (Continuation *C, Environment *env, ptr args)
	{
		env->define(A, args);
		return C->supply(&inert);
	}, A));

	return apply(nc, env, &Eval, list(cadr(args)));
}

Global<C_operative> Scarlett::Define("$define!", internal_define);

