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

Continuation *internal_eval(Continuation *cc, Environment *env, ptr expr)
{
//	std::cerr << "eval: " << expr->repr() << std::endl;
	expr = car(expr);

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
			return eval_args_and_apply(cc, env, cast_ptr<Combiner>(comb), args);
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

		throw Exception(ERROR, "this ain't an applicable thing 'ere: ", comb->repr());
	}

	return cc->supply(expr);
}

Global<C_operative> Scarlett::Eval("$eval", internal_eval);

