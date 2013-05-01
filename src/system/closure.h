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

#pragma once
#include "../object.h"
#include "system.h"
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

