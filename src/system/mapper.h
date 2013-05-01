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
