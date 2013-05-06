#include "system/system.h"
#include "system/global.h"

#ifdef UNITTEST
#include "unittest/test.h"

using namespace Scarlett;

class TbS
{
	int c;

	public:
		TbS(int c_): c(c_) {}

		int plus(int a, int b)
		{ return c * (a + b); }

		int minus(int a, int b)
		{ return c * (a - b); }
};

template <typename T, typename ...Args>
T create(Args &&...args)
{
	return T(std::forward<Args>(args)...);
}

class C_Object: public Operative
{
	Environment *env;
	ptr instance;

	public:
		C_Object(Environment *env_, ptr instance_):
			env(env_), instance(instance_)
		{}

		virtual void gc(Edict const &cmd)
		{
			cmd(env), cmd(instance);
		}

		Continuation *operator()(Continuation *C, Environment *dyn_env, ptr args)
		{
			assert_that("schmob", args, Congruent_with("(<symbol> . <args>)"));
			sym = car(args);
			args = cdr(args);
			return apply(C, env, &Eval, list(list_star(sym, atom_ref(instance), args)));
		}
};

class C_Class: public Applicative
{
	Environment *env;
	CEO constructor;

	public:
		C_Class(CEO const &constructor_, std::map<std::string, CEO> const &methods):
			constructor(constructor_)
		{
			env = new Environment(&nil);
			for (auto &kv : methods)
			{
				env->define(kv.first, new C_Applicative(kv.second));
			}
		}

		virtual void gc(Edict const &cmd)
		{
			cmd(env);
		}

		Continuation *operator()(Continuation *C, Environment *dyn_env, ptr args)
		{
			Continuation *nc = new Apply(C, env, new C_Closure(
				[] (Continuation *C, Environment *env, ptr args)
			{
				// receives an Atom<class T>, making it a C_Object
				return C->supply(new C_Object(
					new Environment(list(env)),
					args));
			});

			return constructor(nc, new Environment(&nil), args);
		}
};

Global<C_Class> Oop_test(
	"oop-test", Curry<TbS, int>(create<TbS, int>),
	{{ "+", Curry<int, TbS *, int, int>(&TbS::plus) },
	 { "-", Curry<int, TbS *, int, int>(&TbS::minus) }} );

Test::Unit Schmob_test(
	"0400 - Schmobs",
	"We would like to talk to objects in Scarlett",
	[] ()
{
	Static<Environment> env(&nil);

	load_global_env(env);
	for (auto &kv : Global<C_Class>::dir())
		env.define(new Symbol(kv.first), kv.second);

	Result r;

	ptr code = "(call/cc ($lambda (c) ((continuation->applicative c) . 42) 12))"_e;

	std::cerr << deep_list_repr(code) << std::endl;
	Program(apply(&r, &env, &Eval, list(cons(&Sequence, code))), &r).run();
	//std::cerr << deep_list_repr(r.result()) << std::endl;
	std::cerr << r.result()->repr() << std::endl;

	return true;	
});

#endif

