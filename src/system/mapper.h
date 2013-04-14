#pragma once

#include "continuation.h"
#include "combiners.h"

namespace Scarlett
{
	class Mapper: public Continuation
	{
		Environment *env;
		Combiner	*func;
		ptr 		src, tgt;

		public:
			void gc(Edict const &cmd) const
			{
				Continuation::gc(cmd);
				cmd(env); cmd(func); cmd(src); cmd(tgt);
			}

			virtual std::string state() const
			{
				std::ostringstream ss;
				ss << "[ Mapper | " << src->repr() << " -> " << tgt->repr() << " ]";
				return ss.str();
			}

			Mapper(Continuation *cc_, Environment *env_, Combiner *func_, ptr args):
				Continuation(cc_), 
				env(env_), func(func_), src(args), tgt(&nil)
			{}

			Continuation *supply(ptr args)
			{
				tgt = cons(args, tgt);
				return this;
			}
			
			Continuation *apply()
			{
				if (is_nil(src))
					return parent()->supply(reverse(tgt));

				ptr expr = list(car(src));
				src = cdr(src);

				return (*func)(this, env, expr);
			}
	};
}

// vim:sw=4:ts=4:tw=72
