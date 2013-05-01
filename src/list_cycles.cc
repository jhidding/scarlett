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

#include "pair.h"
#include "stack.h"

using namespace Scarlett;

#ifdef UNITTEST
#include "unittest/test.h"
#include "read/read.h"
#include "string.h"
#include "system/program.h"

Test::Unit Cycles_test(
	"0201 - cyclic lists",
	"Cyclic lists are hard, thats why Shutt thinks they should be native. "
	"The really hard part is not the naive cyclicity, but the kind of structures "
	"one gets when a car contains a list of references to places up and down "
	"the list. This gives us access to all kind of data topologies covered by "
	"the theory of graphs. There is only one sensible way of dealing with these "
	"structures from the point of view of mutable pairs: marking them. "
	"We currently mark with a char to trace our graph-walking algorithms. "
	"Therefore we need a guarantee that these markers are consistent in between "
	"function calls. ",
	[] ()
{
	ptr q = "(a ...)"_e,
	    p = "(1 2 3 4 5)"_e,
	    a = "((a b) ...)"_e,
	    c = "((a b) (a b) ...)"_e,
	    b = "((1 2) (3 4) (5 6))"_e,
	    d = "(((a 1) (b 2)) (+ a b))"_e,
	    e = "(((<arg1> <expr1>) ...) <body> ...)"_e;

	std::cerr 
		<< deep_list_repr(a) << std::endl 
		<< deep_list_repr(b) << std::endl 
		<< deep_list_repr(c) << std::endl 
		<< deep_list_repr(d) << std::endl 
		<< deep_list_repr(e) << std::endl;

	Test::assert(deep_congruence(car(q), car(p)), " above lists are congruent.");
	Test::assert(deep_congruence(a, b), " above lists are congruent.");
	Test::assert(deep_congruence(q, b), " above lists are congruent.");
	Test::assert(not deep_congruence(a, p), " above lists are not congruent.");
	Test::assert(not deep_compare(b, a), " above lists are not equal.");
	Test::assert(deep_compare(c, a), " above lists are equal.");
	Test::assert(deep_congruence(e, d), " congruence of $let construct.");
	return true;
});

#endif

ptr Scarlett::deep_mark(ptr a, char m)
{
	Static<Stack> sa;
	sa.push(a);

	while (not sa.empty())
	{
		a = sa.top();
		sa.pop();

		if (is_pair(a))
		{
			Pair *aa = cast_ptr<Pair>(a);

			if (aa->mark() != m)
			{
				aa->set_mark(m);
				sa.push(aa->car());
				sa.push(aa->cdr());
			}
		}
	}

	return &inert;
}

bool Scarlett::deep_compare(ptr a_, ptr b_)
{
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sa.empty())
	{
		if (sb.empty())
		{
			deep_mark(a_, 2);
			deep_mark(a_, 0); deep_mark(b_, 0);
			return false;
		}

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_pair(a))
		{
			if (not is_pair(b)) 
			{
				deep_mark(a_, 2); deep_mark(b_, 2);
				deep_mark(a_, 0); deep_mark(b_, 0);
				return false;
			}

			if (mark(a) != 1 or mark(b) != 1)
			{
				set_mark(a, 1); set_mark(b, 1);
				// we go depth first
				sa.push(cdr(a)); sb.push(cdr(b));
				sa.push(car(a)); sb.push(car(b));
			}

			continue;
		}

		if (not is_equiv(a, b)) 
		{
			deep_mark(a_, 2); deep_mark(b_, 2);
			deep_mark(a_, 0); deep_mark(b_, 0);
			return false;
		}
	}

	if (not sb.empty())
	{
		deep_mark(b_, 2);
		deep_mark(a_, 0); deep_mark(b_, 0);
		return false;
	}

	deep_mark(a_, 0); deep_mark(b_, 0);
	return true;
}

bool Scarlett::deep_congruence(ptr a_, ptr b_)
{
	Static<Stack> sa, sb;
	sa.push(a_); sb.push(b_);

	while (not sb.empty())
	{
		if (sa.empty())
		{
			deep_mark(b_, 2);
			deep_mark(a_, 0); deep_mark(b_, 0);
			return false;
		}

		ptr a = sa.top(), b = sb.top();
		sa.pop(); sb.pop(); 

		if (is_pair(a))
		{
			if (is_nil(b) and mark(a) == 1)
				continue;

			if (not is_pair(b)) 
			{
				deep_mark(a_, 2); deep_mark(b_, 2);
				deep_mark(a_, 0); deep_mark(b_, 0);
				return false;
			}

			if (mark(a) != 1 or mark(b) != 1)
			{
				set_mark(a, 1); set_mark(b, 1);
				// we go depth first
				sa.push(cdr(a)); sb.push(cdr(b));
				sa.push(car(a)); sb.push(car(b));
			}

			continue;
		}
	}

	if (not sa.empty())
	{
		deep_mark(a_, 2);
		deep_mark(a_, 0); deep_mark(b_, 0);
		return false;
	}

	deep_mark(a_, 0); deep_mark(b_, 0);
	return true;
}

std::string Scarlett::deep_list_repr(ptr a_)
{
	std::ostringstream ss;
	ss << "(";

	Static<Stack> s_cdr;
	s_cdr.push(a_);

	while (not s_cdr.empty())
	{
		ptr a = s_cdr.top();
		s_cdr.pop();

		//std::cerr << "|d " << a->repr() << "|\n";

		if (is_nil(a))
		{
			ss << "\b) ";
			continue;
		}

		if (not is_pair(a))
		{
			ss << ". " << a->repr() << ") ";
			continue;
		}

		if (mark(a) == 1)
		{
			ss << "...) ";
			continue;
		}

		set_mark(a, 1);
		s_cdr.push(cdr(a));
		a = car(a);
		//std::cerr << "|a " << a->repr() << "|\n";

		while (is_pair(a))
		{
			if (mark(a) == 1) 
			{
				ss << "... ";
				break;
			}

			set_mark(a, 1);
			ss << "(";
			
			s_cdr.push(cdr(a));
			a = car(a);
			//std::cerr << "|a " << a->repr() << "|\n";
		}

		ss << a->repr() << " ";
	}

	//ss << "\b)";
	deep_mark(a_, 0);
	return ss.str();
}

