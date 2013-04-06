#ifdef UNITTEST
#include "unittest/test.h"
#include "pair.h"

using namespace Scarlett;

Test::Unit Pair_test(
	"0011 - pair",
	"The pair is the central object in any Scheme. It is the basis "
	"of every more complex structure, in particular the list. "
	"If we compare this to C++, the pair allows us to iterate "
	"the list, where car gives us the current element pointed to, "
	"and cdr increments it to the next position.\n "
	"Basic law: \n\t(car (cons a b)) := a\n\t(cdr (cons a b)) := b\n",
	[] ()
{
	Test::assert(car(cons("a"_s, "b"_s))->is_equiv("a"_s),
		"(eq? a (car (cons a b)))");
	Test::assert(cdr(cons("a"_s, "b"_s))->is_equiv("b"_s),
		"(eq? b (cdr (cons a b)))");

	auto lst = list(1_a, 2_a, 3_a, 4_a, 5_a);
	Test::assert(car(lst)->is_equiv(1_a),
		"(eq? (car lst) 1)");
	Test::assert(cadr(lst)->is_equiv(2_a),
		"(eq? (cadr lst) 2)");
	Test::assert(caddr(lst)->is_equiv(3_a),
		"(eq? (caddr lst) 3)");
	// etc...

	return true;
});

#endif

