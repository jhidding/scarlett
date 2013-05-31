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
#include <utility>
#include "system.h"
#include "global.h"
#include "safety.h"

namespace Scarlett
{
	/*! wrapping a C++ object.
	 *
	 * \c C_object behaves as a Scarlett operative defined as follows
	 * \code{.scm}
	 * ($define! make-object 
	 *   ($lambda ()
	 *     ($let ((env (current-environment)))
	 *       ($vau (method . args) dyn_env
	 *         ($cond 
	 * 	  ((combiner? (look-up method))
	 * 	   (eval [method . args] env))
	 * 
	 * 	  (#t (look-up method)))))))
	 * \endcode
	 * With the addition that the environment encapsulated thusly,
	 * derives from the environment of a \link C_class , inheriting
	 * its methods.
	 */
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

	/*! Wrapper for native C++ classes
	 *
	 * The class \c C_class is a wrapper around any class defined in C++.
	 * It takes a single template argument, which should be the class
	 * being wrapped. The constructor takes two arguments:
	 * @param constructor a \link CEO pointing to the constructor of the class.
	 * @param methods an \c initializer_list, each item being a pair containing
	 * a string and a \link CEO.
	 *
	 * The resulting object is an \link Applicative that takes the same
	 * arguments as the given constructor, returning a \link C_object.
	 */
	template <typename T>
	class C_class: public Applicative
	{
		Environment *env;
		CEO constructor;

		public:
			template <typename ...I>
			C_class(CEO const &constructor_, std::initializer_list<std::pair<std::string, CEO>> const &methods):
				constructor(constructor_)
			{
				//std::map<std::string, CEO> methods(std::forward<I>(m)...);
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

