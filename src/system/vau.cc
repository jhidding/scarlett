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
#include "closure.h"
#include "safety.h"

using namespace Scarlett;

Continuation *internal_vau(Continuation *C, Environment *env, ptr args)
{
	assert_that("vau", args, Is_proper_list());

	ptr tree 	= car(args);
	ptr dyn_env_sym = cadr(args);
	ptr body 	= cddr(args);

	return C->supply(new Closure(env, dyn_env_sym, tree, copy_es_immutable(body)));
}

Continuation *internal_lambda(Continuation *C, Environment *env, ptr args)
{
	assert_that("lambda", args, Is_proper_list());

	ptr tree = car(args);
	ptr body = cdr(args);

	return C->supply((new Closure(env, &ignore, tree, copy_es_immutable(body)))->wrap());
}

Global<C_operative> Scarlett::Vau("$vau", internal_vau);
Global<C_operative> Scarlett::Lambda("$lambda", internal_lambda);

