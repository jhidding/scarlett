#include "system.h"
#include "safety.h"

using namespace Scarlett;

Continuation *internal_define(Continuation *C, Environment *env, ptr args)
{
	assert_that("$define!", args, Has_proper_length(2));

	ptr A = car(args);
	ptr B = cadr(args);

	env->define(A, B);

	return C->supply(&inert);
}

Global<C_operative> Scarlett::Define("$define!", internal_define);

