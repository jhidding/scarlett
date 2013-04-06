#include "system.h"

using namespace Scarlett;

Continuation *internal_apply(Continuation *C, Environment *env, ptr args)
{
	Applicative *app = cast_ptr<Applicative>(car(args));
	ptr obj = cadr(args);
	Environment *new_env = (is_nil(cddr(args)) ? 
			new Environment(&nil) : 
			cast_ptr<Environment>(caddr(args)));

	return apply(C, new_env, app, obj);
}

Global<C_applicative> ApplyFunc("apply", internal_apply);

