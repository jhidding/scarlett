#include "system.h"

using namespace Scarlett;

class Multi_map: public Continuation
{
	Environment *env;
	Applicative *app;
	ptr src, tgt;

	public:
		Multi_map(Continuation *C, Environment *env_, ptr src):
			Continuation(C), env(env_), src(src_), tgt(&nil)
		{
			int l = length(car(src));
			for (ptr a : List(cdr(src)))
			{
				if (length(a) != l)
					throw Exception(ERROR, "arguments to map should have same number of items.");
			}
		}

		void gc(Edict const &cmd) const
		{ 
			Continuation::gc(cmd); cmd(env); 
			cmd(app); cmd(src); cmd(tgt);
		}

		Continuation *supply(ptr a)
		{
			tgt = cons(a, tgt);
			return this;
		}

		Continuation *apply()
		{

		}
};


Continuation *internal_map(Continuation *C, Environment *env, ptr args)
{
	assert_that("map", args, Congruent_with("((items ...) ...)"_e));


}

Global<C_applicative> Scarlett::Map("map", internal_map);

