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

#include "continuation.h"
#include "safety.h"

using namespace Scarlett;

#ifdef UNITTEST
#include "../unittest/test.h"
#include "program.h"

Test::Unit Continuation_test(
	"0300 - call/cc",
	"call/cc calls the argument applicative with the current "
	"continuation as its single argument.",
	[] ()
{
	Static<Environment> env(&nil);
	load_global_env(env);
	Result r;

	ptr code = "(call/cc ($lambda (c) ((continuation->applicative c) . 42) 12))"_e;

	std::cerr << deep_list_repr(code) << std::endl;
	Program(apply(&r, &env, &Eval, list(cons(&Sequence, code))), &r).run();
	//std::cerr << deep_list_repr(r.result()) << std::endl;
	std::cerr << r.result()->repr() << std::endl;
	
	return true;	
});

#endif

Continuation *internal_call_cc(Continuation *C, Environment *env, ptr args)
{
	assert_that("call/cc", args, Congruent_with("(<combiner>)"_e));
	Combiner *comb = cast_ptr<Combiner>(car(args));

	return apply(C, env, comb, list(C));
}

Continuation *internal_extend_continuation(Continuation *C, Environment *env, ptr args)
{
	assert_that("extend-continuation", args, 
		   Congruent_with("(<continuation> <applicative> <environment>)"_e)
		or Congruent_with("(<continuation> <applicative>"_e));

	Continuation *e_C = cast_ptr<Continuation>(car(args));
	Applicative *e_app = cast_ptr<Applicative>(cadr(args));
	Environment *e_env = (is_nil(cddr(args)) ?
		new Environment(&nil) :
		cast_ptr<Environment>(caddr(args)));

	return C->supply(new Apply(e_C, e_env, e_app)); 
}

/*
Continuation *internal_guard_continuation(Continuation *C, Environment *env, ptr args)
{
	assert_that("guard-continuation", args,
		Congruent_with(
			"(((<continuation> <applicative>) ...)"
			" <continuation>"
			" ((<continuation> <applicative>) ...))"_e));

	ptr entry_guards = copy_es_immutable(car(args));
	Continuation *parent = cast_ptr<Continuation>(cadr(args));
	ptr exit_guards = copy_es_immutable(caddr(args));

	Continuation *outer,
		     *inner;

	return C->supply(inner);
}
*/

Continuation *internal_continuation_to_applicative(Continuation *C, Environment *env, ptr args)
{
	assert_that("continuation->applicative", args,
		Congruent_with("(<continuation>)"_e));

	Continuation *cc = cast_ptr<Continuation>(car(args));
	return C->supply((new C_closure(
		[cc] (Continuation *source, Environment *env, ptr args)
	{
		return cc->abnormal_pass(source, args);
	}, list(cc)))->wrap());
}

Global<C_applicative> Scarlett::Call_CC(
	"call/cc", internal_call_cc);

Global<C_applicative> Scarlett::Extend_continuation(
	"extend-continuation", internal_extend_continuation);
/*
Global<C_applicative> Scarlett::Guard_continuation(
	"guard-continuation", internal_guard_continuation);
*/
Global<C_applicative> Scarlett::Continuation_to_applicative(
	"continuation->applicative", internal_continuation_to_applicative);

