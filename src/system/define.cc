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
#include "safety.h"
#include "continuation.h"
#include "combiners.h"

using namespace Scarlett;

Continuation *internal_define(Continuation *C, Environment *env, ptr args)
{
	assert_that("$define!", args, Has_proper_length(2));

	ptr A = car(args);
	Continuation *nc = new Apply(C, env, new C_closure(
		[A] (Continuation *C, Environment *env, ptr args)
	{
		env->define(A, args);
		return C->supply(&inert);
	}, A));

	return apply(nc, env, &Eval, list(cadr(args)));
}

Global<C_operative> Scarlett::Define("$define!", internal_define);

