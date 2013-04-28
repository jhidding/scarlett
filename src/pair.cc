#include "pair.h"
#include "iface.h"
#include "system/system.h"

using namespace Scarlett;

Global<C_applicative> Is_pair("pair?", Curry<bool, ptr>(is_pair));
Global<C_applicative> Cons("cons", Curry<ptr, ptr, ptr>(cons));
Global<C_applicative> Car("car", Curry<ptr, ptr>(car));
Global<C_applicative> Cdr("cdr", Curry<ptr, ptr>(cdr));

Global<C_applicative> Cadr("cadr", Curry<ptr, ptr>(cadr));
Global<C_applicative> Cddr("cddr", Curry<ptr, ptr>(cddr));
Global<C_applicative> Caar("caar", Curry<ptr, ptr>(caar));
Global<C_applicative> Cdar("cdar", Curry<ptr, ptr>(cdar));

Global<C_applicative> Caddr("caddr", Curry<ptr, ptr>(caddr));
Global<C_applicative> Cdddr("cdddr", Curry<ptr, ptr>(cdddr));
Global<C_applicative> Caadr("caadr", Curry<ptr, ptr>(caadr));
Global<C_applicative> Cdadr("cdadr", Curry<ptr, ptr>(cdadr));
Global<C_applicative> Cadar("cadar", Curry<ptr, ptr>(cadar));
Global<C_applicative> Cddar("cddar", Curry<ptr, ptr>(cddar));
Global<C_applicative> Caaar("caaar", Curry<ptr, ptr>(caaar));
Global<C_applicative> Cdaar("cdaar", Curry<ptr, ptr>(cdaar));

Global<C_applicative> Cadddr("cadddr", Curry<ptr, ptr>(cadddr));
Global<C_applicative> Cddddr("cddddr", Curry<ptr, ptr>(cddddr));
Global<C_applicative> Caaddr("caaddr", Curry<ptr, ptr>(caaddr));
Global<C_applicative> Cdaddr("cdaddr", Curry<ptr, ptr>(cdaddr));
Global<C_applicative> Cadadr("cadadr", Curry<ptr, ptr>(cadadr));
Global<C_applicative> Cddadr("cddadr", Curry<ptr, ptr>(cddadr));
Global<C_applicative> Caaadr("caaadr", Curry<ptr, ptr>(caaadr));
Global<C_applicative> Cdaadr("cdaadr", Curry<ptr, ptr>(cdaadr));
Global<C_applicative> Caddar("caddar", Curry<ptr, ptr>(caddar));
Global<C_applicative> Cdddar("cdddar", Curry<ptr, ptr>(cdddar));
Global<C_applicative> Caadar("caadar", Curry<ptr, ptr>(caadar));
Global<C_applicative> Cdadar("cdadar", Curry<ptr, ptr>(cdadar));
Global<C_applicative> Cadaar("cadaar", Curry<ptr, ptr>(cadaar));
Global<C_applicative> Cddaar("cddaar", Curry<ptr, ptr>(cddaar));
Global<C_applicative> Caaaar("caaaar", Curry<ptr, ptr>(caaaar));
Global<C_applicative> Cdaaar("cdaaar", Curry<ptr, ptr>(cdaaar));

Global<C_applicative> Is_it_proper_list("list-proper?", Curry<bool, ptr>(is_proper_list));
Global<C_applicative> Reverse("reverse", Curry<ptr, ptr>(reverse));

Continuation *internal_list(Continuation *C, Environment *env, ptr args)
{ return C->supply(args); }

Continuation *internal_list_star(Continuation *C, Environment *env, ptr args)
{
	ptr rev = reverse(args);
	ptr result = cons(cadr(rev), car(rev));
	rev = cddr(rev);

	while (not is_nil(rev))
	{
		result = cons(car(rev), result);
		rev = cdr(rev);
	}

	return C->supply(result);
}

#include "listfunc.h"
Global<C_applicative> Scarlett::ListFunc("list", internal_list);
Global<C_applicative> ListStarFunc("list*", internal_list_star);

Global<C_applicative> Set_car("set-car!", Curry<ptr, ptr, ptr>(set_car));
Global<C_applicative> Set_cdr("set-cdr!", Curry<ptr, ptr, ptr>(set_cdr));
Global<C_applicative> Copy_es_immutable("copy-es-immutable", Curry<ptr, ptr>(copy_es_immutable));

