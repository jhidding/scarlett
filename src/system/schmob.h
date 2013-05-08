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
#include "system.h"
#include "global.h"
#include "safety.h"

namespace Scarlett
{
	template <typename T>
	class C_object: public Operative
	{
		Environment *env;
		ptr instance;

		public:
			C_object(Environment *env_, ptr instance_):
				env(new Environment(list(env_))), instance(instance_)
			{}

			virtual void gc(Edict const &cmd) const
			{
				cmd(env); cmd(instance);
			}

			Continuation *operator()(Continuation *C, Environment *dyn_env, ptr args)
			{
				assert_that("schmob", args, Congruent_with("(<symbol> . <args>)"_e));
				ptr sym = car(args);
				args = cdr(args);

				ptr self = new Atom<T *>(&(cast_ptr<Atom<T>>(instance)->ref()));
				return apply(C, env, &Eval, list(list_star(
						sym, self, args)));
			}
	};

	template <typename T>
	class C_class: public Applicative
	{
		Environment *env;
		CEO constructor;

		public:
			C_class(CEO const &constructor_, std::map<std::string, CEO> const &methods):
				constructor(constructor_)
			{
				env = new Environment(&nil);
				for (auto &kv : methods)
				{
					env->define(kv.first, new C_applicative(kv.second));
				}
			}

			virtual void gc(Edict const &cmd) const
			{
				cmd(env);
			}

			Continuation *operator()(Continuation *C, Environment *dyn_env, ptr args)
			{
				Continuation *nc = new Apply(C, env, new C_closure(
					[] (Continuation *C, Environment *env, ptr args)
				{
					// receives an Atom<class T>, making it a C_Object
					return C->supply(new C_object<T>(env, args));
				}));

				return constructor(nc, new Environment(&nil), args);
			}
	};
}

