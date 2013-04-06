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

