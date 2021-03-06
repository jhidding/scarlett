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

#include "system.h"
#include "../pair.h"

using namespace Scarlett;

class ApplyWith: public Operative
{
	ptr args;

	public:
		void gc(Edict const &cmd) const { cmd(args); }
		ApplyWith(ptr args_): args(args_) {}

		Continuation *operator()(Continuation *C,
			Environment *env, ptr opp)
		{
			return apply(C, env, &Eval, list(cons(opp, args))); 
		}
};

inline Continuation *eval_args_and_apply(Continuation *cc, Environment *env, 
		Applicative *comb, ptr args)
{
	Continuation *nc = new Apply(cc, env, comb);

	if (is_pair(args))
	{	
		return new Multi_map(nc, env, &Eval, list(args));
	}

	return apply(nc, env, &Eval, list(args));
}

Continuation *internal_eval(Continuation *cc, Environment *env, ptr args_)
{
//	std::cerr << "eval: " << deep_list_repr(args_) << std::endl;
	ptr expr = car(args_);
	if (is_pair(cdr(args_)))
	{
		assert_that("$eval", cadr(args_), Is_environment());
		env = cast_ptr<Environment>(cadr(args_));
	}

	if (is_symbol(expr)) 
		return cc->supply(env->look_up(expr));

	if (is_pair(expr))
	{
		ptr comb = car(expr), args = cdr(expr);

		if (is_symbol(comb))
		{
			comb = env->look_up(comb);
		}

		if (is_applicative(comb))
		{
			return eval_args_and_apply(cc, env, cast_ptr<Applicative>(comb), args);
		}
		
		if (is_operative(comb))	
		{
			return apply(cc, env, cast_ptr<Combiner>(comb), args);
		}

		if (is_pair(comb))
		{
			Continuation *nc = new Apply(cc, env, new ApplyWith(args));
			return apply(nc, env, &Eval, list(comb));
		}

		throw Exception(ERROR, "this ain't an applicable thing 'ere: ", comb->repr(), ". evaluating: ", repr(args_));
	}

	return cc->supply(expr);
}

Global<C_operative> Scarlett::Eval("$eval", internal_eval);
Global<C_applicative> Eval_app("eval", internal_eval);

