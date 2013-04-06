#pragma once
#include "../object.h"
#include "environment.h"
#include "continuation.h"
#include "combiners.h"

namespace Scarlett
{
	class Closure: public Operative
	{
		Environment *stat_env;
		ptr dyn_name, args, body;

		public:
			void gc(Edict const &cmd) const
				{ cmd(stat_env); cmd(dyn_name); cmd(args); cmd(body); }

			Closure(Environment *env_, ptr dyn_name_, ptr args_, ptr body_):
				stat_env(env_), dyn_name(dyn_name_),
				args(args_), body(body_)
			{}

			Continuation *operator()(Continuation *C, Environment *dyn_env, ptr pars)
			{
				Environment *env = new Environment(args, pars, list(stat_env));
				env->define(dyn_name, dyn_env);
				return apply(C, env, &Eval, list(cons(&Sequence, body)));
			}
	};
}

