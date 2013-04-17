#include "continuation.h"
#include "safety.h"

using namespace Scarlett;

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
		or Congrunet_with("(<continuation> <applicative>"_e));

	Continuation *e_C = cast_ptr<Continuation>(car(args));
	Applicative *e_app = cast_ptr<Applicative>(cadr(args));
	Environment *e_env = (is_nil(cddr(args)) ?
		new Environment(&nil) :
		cast_ptr<Environment>(caddr(args)));

	return C->supply(new Apply(e_C, e_env, e_app)); 
}

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

Continuation *internal_continuation_to_applicative(Continuation *C, Environment *env, ptr args)
{
	assert_that("continuation->applicative", args,
		Congruent_with("(<continuation>)"_e));

	Continuation *cc = cast_ptr<Continuation>(car(args));
	return C->supply((new C_closure(
		[cc] (Continuation *C, Environment *env, ptr args)
	{
		return cc->supply(args);
	}, list(cc)))->wrap());
}

Global<C_applicative> Scarlett::Call_CC(
	"call/cc", internal_call_cc);

Global<C_applicative> Scarlett::Extend_continuation(
	"extend-continuation", internal_extend_continuation);

Global<C_applicative> Scarlett::Guard_continuation(
	"guard-continuation", internal_guard_continuation);

Global<C_applicative> Scarlett::Continuation_to_applicative(
	"continuation->applicative", internal_continuation_to_applicative);

