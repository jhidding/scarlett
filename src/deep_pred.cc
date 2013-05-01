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

#include "pair.h"
#include "system/system.h"

using namespace Scarlett;

class Congruence_inquisitor: public Continuation
{
	Environment *env;
	Stack *sa, *sb;
	bool result;

	public:
		Congruence_inquisitor(Continuation *parent_, Environment *env_, ptr a_, ptr b_):
			Continuation(parent_), env(env_),
			sa(new Stack), sb(new Stack),
			result(true)
		{
			sa->push(copy_es_immutable(a_));
			sb->push(copy_es_immutable(b_));
		}

		virtual void gc(Edict const &cmd) const
		{
			Continuation::gc(cmd);
			cmd(sa); cmd(sb);
		}

		virtual Continuation *supply(ptr a)
		{
			assert_that("congruent? | predicate", a, Is_boolean());
			result = cast_ptr<Boolean>(a)->value();
			std::cerr << "got " << (result ? "true" : "false") << " in congruent? | predicate.\n";
			return this;
		}

		virtual Continuation *apply()
		{
			if (not result)
				return parent()->supply(new Boolean(false));

			if (not sb->empty())
			{
				if (sa->empty())
					return parent()->supply(new Boolean(false));

				ptr a = sa->top(), b = sb->top();
				sa->pop(); sb->pop();

				if (is_pair(a))
				{
					if (is_nil(b) and mark(a) == 1)
						return this;

					if (not is_pair(b)) 
						return parent()->supply(new Boolean(false));

					if (mark(a) != 1 or mark(b) != 1)
					{
						set_mark(a, 1); set_mark(b, 1);

						// we go depth first
						sa->push(cdr(a)); sb->push(cdr(b));
						sa->push(car(a)); sb->push(car(b));
					}

					return this;
				}

				if (is_applicative(a))
				{
					return (*cast_ptr<Applicative>(a))(this, env, list(b));
				}

				return this;
			}

			if (not sa->empty())
				return parent()->supply(new Boolean(false));
			
			// finally!
			return parent()->supply(new Boolean(true));
		}
};

Continuation *deep_pred(Continuation *C, Environment *env, ptr args)
{
	assert_that("congruent?", args, Has_proper_length(2));
	return new Congruence_inquisitor(C, env, car(args), cadr(args));
}

Global<C_applicative> Is_congruent("congruent?", deep_pred);

