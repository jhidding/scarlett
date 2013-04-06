#include "system.h"
#include "safety.h"
#include "../misc/format.h"

using namespace Scarlett;

class Sequencer: public Continuation
{
	Environment 	*env;
	ptr		lst;

	public:
		void gc(Edict const &cmd) const
		{
			Continuation::gc(cmd);
			cmd(env); cmd(lst);
		}

		Sequencer(Continuation *parent_, Environment *env_, ptr lst_):
			Continuation(parent_), env(env_), lst(lst_) {}

		std::string state() const
		{
			return Misc::format("[ Sequence | ", lst->repr(), " ]");
		}

		Continuation *supply(ptr args)
		{
			// forget it
			return this;
		}

		Continuation *apply()
		{
			ptr A = car(lst), B = cdr(lst);
			if (is_nil(B))
			{
				return Scarlett::apply(parent(), env, &Eval, list(A));
			}
			else
			{
				lst = B;
				return Scarlett::apply(this, env, &Eval, list(A));
			}
		}
};

Continuation *internal_sequence(Continuation *C, Environment *env, ptr args)
{
	assert_that("$sequence", args, Is_proper_list());

	if (is_nil(args))
		return C->supply(&inert);

	Continuation *nc = new Sequencer(C, env, args);

	return nc;
}

Global<C_operative> Scarlett::Sequence("$sequence", internal_sequence);

