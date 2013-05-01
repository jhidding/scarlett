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

Continuation *internal_wrap(Continuation *C, Environment *env, ptr args)
{
	Combiner *opp = cast_ptr<Combiner>(car(args));

	return C->supply(opp->wrap());
}

Continuation *internal_unwrap(Continuation *C, Environment *env, ptr args)
{
	Applicative *app = cast_ptr<Applicative>(car(args));

	return C->supply(app->unwrap());
}

Global<C_applicative> Scarlett::Wrap("wrap", internal_wrap);
Global<C_applicative> Scarlett::Unwrap("unwrap", internal_unwrap);

