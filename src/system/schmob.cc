/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#include "schmob.h"

#ifdef UNITTEST
#include "../unittest/test.h"
#include "../iface.h"

using namespace Scarlett;

class TbS
{
	int c;

	public:
		TbS(int c_): c(c_) 
		{
			std::cerr << "initialized " << c << std::endl;
		}

		int multiplier() const 
		{ return c; }

		int plus(int a, int b)
		{ return c * (a + b); }

		int minus(int a, int b)
		{ return c * (a - b); }
};

std::ostream &operator<<(std::ostream &out, TbS const &obj)
{
	return out << "[TbS " << obj.multiplier() << "]";
}

Test::Unit Schmob_test(
	"0400 - Schmobs",
	"We would like to talk to objects in Scarlett",
	[] ()
{
	Static<Environment> env(&nil);

	load_global_env(env);
//	for (auto &kv : Global<C_class<TbS>>::dir())
//		env.define(new Symbol(kv.first), kv.second);

	Result r;

	Static<C_class<TbS>> Oop_test(
		C_class<TbS>(Curry<TbS, int>(create<TbS, int>),
		{ { "+", Curry<int, TbS *, int, int>(&TbS::plus)  },
		  { "-", Curry<int, TbS *, int, int>(&TbS::minus) } }));

	env.define("oop-test", &Oop_test);

	ptr code = "($let ((A (oop-test 5)) (B (oop-test 7)))"
		"(display (A + 1 2) \" \" (B + 3 4) \"\\n\"))"_e;

	std::cerr << deep_list_repr(code) << std::endl;
	Program(apply(&r, &env, &Eval, list(cons(&Sequence, code))), &r).run();
	//std::cerr << deep_list_repr(r.result()) << std::endl;
	std::cerr << repr(r.result()) << std::endl;

	return true;	
});

#endif

