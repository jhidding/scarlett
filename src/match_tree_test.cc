#ifdef UNITTEST
#include "unittest/test.h"
#include "pair.h"
#include <map>
#include <string>

using namespace Scarlett;

class TestEnv: public std::map<std::string, ptr>
{
	public:
		void operator()(ptr a, ptr b)
		{
			std::cerr << "matching: " << a->repr()
				<< " to " << b->repr() << std::endl;

			insert(std::pair<std::string, ptr>(
				cast_ptr<Symbol>(a)->value(), b));
		}
};

Test::Unit Match_tree_test(
	"0012 - match tree",
	"Matches the skeleton structure of two trees. "
	"The first one is the argument tree and contains "
	"the symbols to be bound. "
	"The second contains the values to be assigned "
	"to these variables. "
	"The argument tree is a pair (A . B), where "
	"A is either a symbol or an argument tree, "
	"and B is a symbol, an argument tree or #nil. "
	"The second tree should be congruent to the "
	"first one; if in the first one we encounter "
	"a list, the second should be a list as well. "
	"If the item in the first one is a symbol, "
	"the second may be anything at all. "
	"If we encounter #nil in the argument tree, "
	"the match should be #nil. "
	"The function takes two pointers and a function "
	"of type void (ptr, ptr).",
	[] ()
{
	TestEnv env;
	match_tree(list(list("a1"_s, "a2"_s), "b"_s, list("c"_s, "d"_s)), 
			list(list(1_a, 1.5_a), 2_a, list(3_a, 4_a)),
		[&env] (ptr a, ptr b)
	{
		env[a->repr()] = b;
	});

	for (auto kv : env)
	{
		std::cerr << kv.first << " -> " << kv.second->repr() << " | ";
	}
	std::cerr << std::endl;

	Test::assert(cast_ptr<Atom<unsigned>>(env["c"])->value() == 3,
			"c should be assigned 3.");

	return true;
});

#endif

