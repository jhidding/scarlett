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
