#include "system.h"
#include "../numeric/numeric.h"

using namespace Scarlett;

#ifdef UNITTEST
#include "../unittest/test.h"

Test::Unit Map_test(
	"0204 - map",
	"another very complicated function, largely because of "
	"cyclic list support. ",
	[] ()
{
	Static<Environment> env(&nil);
	load_global_env(env);
	Result r;

	ptr code = "(map ($lambda (x y) (+ x y)) (list 1 2 3 ...) "
		"(list 10 20 30 40 50 ...))"_e;
	std::cerr << deep_list_repr(code) << std::endl;
	Program(apply(&r, &env, &Eval, list(cons(&Sequence, code))), &r).run();
	std::cerr << deep_list_repr(r.result()) << std::endl;
	
	return true;	
});
#endif

std::string Multi_map::state() const
{
	return Misc::format("[ Map | ", ::repr(src), " -> ", ::repr(tgt), " ]");
}

Multi_map::Multi_map(Continuation *C, Environment *env_, Combiner *app_, ptr src_):
	Continuation(C), env(env_), app(app_), src(src_), tgt(&nil), 
	improper(false), l(0)
{
	src_metric = c_get_list_metric(src);

	std::vector<List_metric> A;
	for (ptr a : List(src))
	{
		A.push_back(c_get_list_metric(a));
	}

	// cheap bug-fix, List iterator and get_list_metric
	// don't work together well...
	deep_mark(src, 3);
	deep_mark(src, 0);

	tgt_metric = { A[0].p, A[0].n, 0, A[0].c };
	if (tgt_metric.c == 0)
	{
		for (List_metric &M : A)
		{
			if (not (M.c == 0 and M.p == tgt_metric.p and M.n == tgt_metric.n))
				throw Exception(ERROR, "map arguments should have same length.");
		}

		tgt_metric.a = tgt_metric.p; 
	}
	else
	{
		for (List_metric &M : A)
		{
			if (M.c == 0)
				throw Exception(ERROR, "if one map argument is cyclic, all must be.");

			tgt_metric.a = max(tgt_metric.a, M.a);
			tgt_metric.c = lcm(tgt_metric.c, M.c);
		}

		tgt_metric.p = tgt_metric.a + tgt_metric.c;
		tgt_metric.n = 0;
	}

	if (tgt_metric.n == 0 and tgt_metric.c == 0)
	{
		improper = true;
	}
}

void Multi_map::gc(Edict const &cmd) const
{ 
	Continuation::gc(cmd); cmd(env); 
	cmd(app); cmd(src); cmd(tgt);
}

Continuation *Multi_map::supply(ptr a)
{
	tgt = cons(a, tgt);
	return this;
}

Continuation *internal_improper(Continuation *p, Environment *env, ptr lst)
{
	return new Apply(p, env, new C_closure(
		[lst] (Continuation *C, Environment *env, ptr args)
	{
		return C->supply(improper_reverse(lst, args));
	}, lst));
}

Continuation *Multi_map::apply()
{
	if (l == tgt_metric.p)
	{
		if (improper)
		{
			ptr src_car = &nil;
			for (ptr a : List(src))
			{
				src_car = cons(a, src_car);
			}

			src_car = reverse(src_car);
			encycle(src_car, src_metric.a, src_metric.c);
			return (*app)(internal_improper(parent(), env, tgt), env, src_car);
		}

		tgt = reverse(tgt); 
		encycle(tgt, tgt_metric.a, tgt_metric.c);
		return parent()->supply(tgt);
	}

	ptr src_car = &nil, src_cdr = &nil;
	for (ptr a : List(src))
	{
		src_car = cons(car(a), src_car);
		src_cdr = cons(cdr(a), src_cdr);
	}

	l++;
	src = reverse(src_cdr);
	src_car = reverse(src_car);
	encycle(src_car, src_metric.a, src_metric.c);
	return (*app)(this, env, src_car);
}

Continuation *internal_map(Continuation *C, Environment *env, ptr args)
{
//	assert_that("map", args, Congruent_with("((items ...) ...)"_e));
	Applicative *app = cast_ptr<Applicative>(car(args));
	return new Multi_map(C, env, app, cdr(args));
}

Global<C_applicative> Scarlett::Map("map", internal_map);

// vim:sw=4:ts=4:tw=72
