#include "environment.h"
#include "continuation.h"
#include "combiners.h"
#include "safety.h"
#include "system.h"

using namespace Scarlett;

Continuation *internal_if(Continuation *C, Environment *env, ptr args)
{
	assert_that("if", args, Has_proper_length(3));

	ptr pred = car(args);
	ptr opt1 = cadr(args);
	ptr opt2 = caddr(args);

	Continuation *nc = new Apply(C, env, new C_closure(
		[opt1,opt2] (Continuation *C, Environment *env, ptr args)
	{
		assert_that("if", args, Is_boolean());

		if (cast_ptr<Boolean>(args)->value())
		{
			return apply(C, env, &Eval, list(opt1));
		}
		else
		{
			return apply(C, env, &Eval, list(opt2));
		}
	}, list(opt1, opt2)));

	return apply(nc, env, &Eval, list(pred));
}

Global<C_operative> Scarlett::If("$if", internal_if);

