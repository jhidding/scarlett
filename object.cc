#include "object.h"
#include "pair.h"

using namespace Scarlett;

std::unique_ptr<GC<Object>> Object::_gc_ptr;

Static<Special> Scarlett::inert(Special("inert"));
Static<Special> Scarlett::ignore(Special("ignore"));
Static<Special> Scarlett::nil(Special("nil"));

bool Scarlett::is_equal(ptr a, ptr b)
{
	if (a->type_name() != b->type_name()) return false;

	if (a->is_mutable())
		return (b->is_mutable() ? a == b : false);

	if (b->is_mutable()) return false;

	return a->is_equiv(b);
}

bool Scarlett::is_equiv(ptr a, ptr b)
{
	if (a->type_name() != b->type_name()) return false;

	if (is_pair(a)) return pair_equiv(a, b);

	return a->is_equiv(b);
}


#include "system/system.h"
#include "iface.h"
Global<C_applicative> Is_boolean("boolean?", Curry<bool, ptr>(is_boolean));
Global<C_applicative> Is_nil("nil?", Curry<bool, ptr>(is_nil));
Global<C_applicative> Is_ignore("ignore?", Curry<bool, ptr>(is_ignore));
Global<C_applicative> Is_inert("inert?", Curry<bool, ptr>(is_inert));
Global<C_applicative> Is_symbol("symbol?", Curry<bool, ptr>(is_symbol));
Global<C_applicative> Is_equiv("equiv?", Curry<bool, ptr, ptr>(is_equiv));
Global<C_applicative> Is_equal("equal?", Curry<bool, ptr, ptr>(is_equal));

#ifdef UNITTEST
#include "unittest/test.h"

Test::Unit Object_Unittest(
	"0010 - Scarlett object system",
	"This module is the basis of the object system. "
	"It should define Object and Static compliant to "
	"the garabage collector. Derived from those: "
	"a template class Atom<T> to encapsulate native C++ "
	"types; Symbol containing any symbol that does not "
	"contain any of '()# '; Special identifying the special "
	"cases of #inert, #ignore and #nil, along with their "
	"respective predicates. Last we define some C++ literals "
	"to create Atom<unsigned> and Atom<double>.",
	[] ()
{
	auto a = 4568_a;
	auto b = 42.9_a;

	if (a->type_name() != "atomic")
		throw Exception(ERROR_fail, "type of variable <a> should be atomic.");

	if (cast_ptr<Atom<unsigned>>(a)->value() != 4568)
		throw Exception(ERROR_fail, "value of variable <a> should be 4568.");

	if (b->repr() != "42.9")
		throw Exception(ERROR_fail, "value of variable <b> should be repr'd as 42.9.");

	if ("blah"_s->type_name() != "symbol")
		throw Exception(ERROR_fail, "type of variable \"blah\"_s should be symbol.");

	return true;
});

#endif

