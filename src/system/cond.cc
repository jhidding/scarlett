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

#include "system.h"

using namespace Scarlett;

#ifdef UNITTEST
#include "../unittest/test.h"

Test::Unit Internal_cond_test(
	"0203 - $cond",
	"$cond is a higher order $if.",
	[] ()
{
	Static<Environment> env(&nil);
	load_global_env(env);
	Result r;

	ptr code = "($cond (#f 3) (#f 4) (#t 5) (#f 6))"_e;
	std::cerr << deep_list_repr(code) << std::endl;
	Program(apply(&r, &env, &Eval, list(cons(&Sequence, code))), &r).run();
	std::cerr << r.result()->repr() << std::endl;
	
	return true;
});

#endif

Continuation *internal_cond(Continuation *C, Environment *env, ptr args)
{
	if (is_nil(args)) return C->supply(&inert);

	assert_that("$cond", args, Congruent_with("((<test> . <body>) ...)"_e));

	ptr pred1 = caar(args),
	    body1 = cdar(args),
	    rest = cdr(args);

	Continuation *nc = new Apply(C, env, new C_closure(
		[body1, rest] (Continuation *C, Environment *env, ptr pred)
	{
		assert_that("$cond", pred, Is_boolean());

		if (cast_ptr<Boolean>(pred)->value())
			return apply(C, env, &Eval, list(cons(&Sequence, body1)));
		
		return internal_cond(C, env, rest);
	}, list(body1, rest)));

	return apply(nc, env, &Eval, list(pred1));
}

Global<C_operative> Scarlett::Cond("$cond", internal_cond);

