#ifdef UNITTEST
#include "../unittest/test.h"
#include "system.h"
#include "mapper.h"
#include "../iface.h"

using namespace Scarlett;

class Program: Static<Object>
{
	Continuation *_begin, *_end;

	public:
		class iterator: public Static<Object>, 
			std::iterator<std::forward_iterator_tag, Continuation>
		{
			Continuation *c;

			public:
				virtual void gc(Edict const &cmd) const { cmd(c); }
				
				iterator(Continuation *c_): c(c_) {}
				iterator &operator++() { c = c->apply(); return *this; }
				bool operator==(iterator &o) const { return c == o.c; }
				bool operator!=(iterator &o) const { return c != o.c; }
				Continuation &operator*() { return *c; }
		};

		Program(Continuation *begin_, Continuation *end_):
			_begin(begin_), _end(end_) {}

		virtual void gc(Edict const &cmd) const { cmd(_begin); cmd(_end); }
		iterator begin() const { return iterator(_begin); }
		iterator end() const { return iterator(_end); }
};

class Result: public Continuation
{
	ptr _result;

	public:
		Result(): Continuation(NULL) {}

		ptr result() const { return _result; }

		virtual Continuation *supply(ptr a)
		{
			_result = a;
			return this;
		}

		virtual Continuation *apply()
		{
			throw Exception(ERROR, "cannot apply top-level continuation.");
		}
};

class Square: public Operative
{
	public:
		virtual Continuation *operator()(Continuation *C,
			Environment *env, ptr args)
		{
			std::cerr << "square getting: " << args->repr() << std::endl;
			unsigned i = cast_ptr<Atom<unsigned>>(car(args))->value();
			return C->supply(new Atom<unsigned>(i*i));
		}
};

int plus(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
bool zero(int a) { return a == 0; }
int min(int a, int b) { return a - b; }
Global<C_applicative> Min("-", Curry<int, int, int>(min));
Global<C_applicative> Mul("*", Curry<int, int, int>(mul));
Global<C_applicative> Zero("zero?", Curry<bool, int>(zero));
Global<C_applicative> Plus("+", Curry<int, int, int>(plus));

Test::Unit Mapper_test(
	"0101 - mapper",
	"Performs a sequential mapping.",
	[] ()
{
	Static<Result> r;
	ptr a = list(1_a, 2_a, 3_a, 4_a, 5_a);

	for (Continuation &cc : Program(new Mapper(&r, NULL, new Square, a), &r))
	{
		std::cerr << cc.state() << std::endl;
		GC<Object>::cycle();
	}

	std::cerr << r.result()->repr() << std::endl;

	return true;
});


Test::Unit List_of_apps_test(
	"0102 - list Globals",
	"lists some global variables",
	[] ()
{
	Static<Environment> env(&nil);

	for (auto &kv : Global<C_applicative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	for (auto &kv : Global<C_operative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	env.print_map(std::cerr);

	Static<Result> r;
	Program program(apply(&r, &env, &Eval, list(
		list(list("$lambda"_s, list("x"_s, "y"_s), 
			list("+"_s, list("+"_s, "x"_s, "x"_s), "y"_s)),
			2_a, 7_a))), &r);
	for (Continuation &cc : program)
	{
		//std::cerr << cc.state() << std::endl;
		GC<Object>::cycle();
	}
	std::cerr << r.result()->repr() << std::endl;

	return true;
});

Test::Unit Factorial_test_1(
	"0103 - simple factorial function",
	"compute 10 factorial by non-tail-recursive way",
	[] ()
{
	Static<Environment> env(&nil);

	for (auto &kv : Global<C_applicative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	for (auto &kv : Global<C_operative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	Static<Result> r;
	Program program(apply(&r, &env, &Eval, list(list("$sequence"_s,
		list("$define!"_s, "fac"_s, list("$lambda"_s, list("n"_s),
			list("$if"_s, list("zero?"_s, "n"_s),
				1_a,
				list("*"_s, "n"_s, list("fac"_s, list("-"_s, "n"_s, 1_a)))))),

		list("fac"_s, 10_a)
	))), &r);
	for (Continuation &cc : program)
	{
		//std::cerr << cc.state() << std::endl;
	}
	std::cerr << r.result()->repr() << std::endl;
	GC<Object>::cycle();

	return true;
});

Test::Unit Factorial_test_2(
	"0104 - slightly less simple factorial function",
	"compute 10 factorial by tail-recursive way",
	[] ()
{
	Static<Environment> env(&nil);

	for (auto &kv : Global<C_applicative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	for (auto &kv : Global<C_operative>::dir())
		env.define(new Symbol(kv.first), kv.second);

	Static<Result> r;
	Program program(apply(&r, &env, &Eval, list(list("$sequence"_s,
		list("$define!"_s, "fac"_s, list("$lambda"_s, list("n"_s),
			list("$define!"_s, "loop"_s, list("$lambda"_s, list("a"_s, "b"_s),
				list("$if"_s, list("zero?"_s, "a"_s),
					"b"_s,
					list("loop"_s, list("-"_s, "a"_s, 1_a), list("*"_s, "b"_s, "a"_s))))),

			list("loop"_s, "n"_s, 1_a))),

		list("fac"_s, 10_a)
	))), &r);

	for (Continuation &cc : program)
	{
		//std::cerr << cc.state() << std::endl;
		//GC<Object>::cycle();
	}

	std::cerr << r.result()->repr() << std::endl;
	GC<Object>::cycle();

	return true;
});

#endif

