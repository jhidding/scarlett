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
#include "../pair.h"
#include "../stack.h"
#include "../read/read.h"
#include "safety.h"
#include "environment.h"
#include "continuation.h"
#include "combiners.h"
#include "mapper.h"
#include "global.h"
#include "multimap.h"
#include "program.h"

namespace Scarlett
{
	extern Global<C_operative> Eval;
	extern Global<C_operative> Vau;
	extern Global<C_operative> Sequence;
	extern Global<C_operative> If;
	extern Global<C_operative> Cond;
	extern Global<C_operative> Define;
	extern Global<C_operative> Lambda;
	extern Global<C_operative> Let;

	extern Global<C_applicative> Wrap;
	extern Global<C_applicative> Unwrap;
	extern Global<C_applicative> Is_applicative;
	extern Global<C_applicative> Is_operative;
	extern Global<C_applicative> Make_environment;
	extern Global<C_applicative> Map;

	extern Global<C_applicative> Call_CC;
	extern Global<C_applicative> Extend_continuation;
	extern Global<C_applicative> Continuation_to_applicative;

	inline Continuation *apply(Continuation *cc, Environment *env, Combiner *comb, ptr args)
	{
		Continuation *nc = new Apply(cc, env, comb);
		return nc->supply(args);
	}
}

// vim:sw=4:ts=4:tw=72
