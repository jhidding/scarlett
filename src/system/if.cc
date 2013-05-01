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

#include "environment.h"
#include "continuation.h"
#include "combiners.h"
#include "safety.h"
#include "system.h"

using namespace Scarlett;

Continuation *internal_if(Continuation *C, Environment *env, ptr args)
{
	assert_that("if", args, Has_proper_length(3));

	ptr pred = car(args);
	ptr opt1 = cadr(args);
	ptr opt2 = caddr(args);

	Continuation *nc = new Apply(C, env, new C_closure(
		[opt1,opt2] (Continuation *C, Environment *env, ptr args)
	{
		assert_that("if", args, Is_boolean());

		if (cast_ptr<Boolean>(args)->value())
		{
			return apply(C, env, &Eval, list(opt1));
		}
		else
		{
			return apply(C, env, &Eval, list(opt2));
		}
	}, list(opt1, opt2)));

	return apply(nc, env, &Eval, list(pred));
}

Global<C_operative> Scarlett::If("$if", internal_if);

