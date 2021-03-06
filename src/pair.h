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

#pragma once

#include "object.h"
#include <iterator>

namespace Scarlett
{
	// class Pair {{{1
	class Pair: public Object
	{
		mutable char _mark;
		bool _mute;

		ptr a, b;

		public:
			Pair(ptr a_, ptr b_, bool mute_ = true): 
				_mark(0), _mute(mute_),
				a(a_), b(b_) 
			{}

			ptr car() const { return a; }
			ptr cdr() const { return b; }

			void set_car(ptr u)
			{ 
				if (not _mute) 
					throw Exception(ERROR, "cannot mutate a non-mutable pair.");
				a = u;
			}

			void set_cdr(ptr v) 
			{ 
				if (not _mute) 
					throw Exception(ERROR, "cannot mutate a non-mutable pair.");
				b = v; 
			}

			void set_mark(char m) const { _mark = m; }
			char mark() const { return _mark; }

			virtual void gc(Edict const &cmd) const
				{ cmd(a); cmd(b); }

			virtual std::string type_name() const { return "pair"; }
			virtual std::string repr() const;
			virtual bool is_mutable() const { return _mute; }
			
			void set_mutable(bool a) { _mute = a; }
			//virtual bool is_equiv(ptr a) const;
	}; // }}}

	// core functions {{{1
	inline bool is_pair(ptr q) { return q->type_name() == "pair"; }
	inline ptr cons(ptr a, ptr b) { return new Pair(a, b); }
	inline ptr car(ptr q) { return cast_ptr<Pair>(q)->car(); }
	inline ptr cdr(ptr q) { return cast_ptr<Pair>(q)->cdr(); }
	inline void set_mark(ptr q, char m) { 
		if (not is_pair(q))
			throw Exception(ERROR, "can't mark ", q->repr());
		cast_ptr<Pair>(q)->set_mark(m); }
	inline char mark(ptr q) { return cast_ptr<Pair>(q)->mark(); }
	inline ptr set_car(ptr q, ptr a) { cast_ptr<Pair>(q)->set_car(a); return &inert; }
	inline ptr set_cdr(ptr q, ptr a) { cast_ptr<Pair>(q)->set_cdr(a); return &inert; }

	inline ptr list() { return &nil; }

	template <typename ...Args>
	inline ptr list(ptr a, Args &&...args)
	{ return cons(a, list(std::forward<Args>(args)...)); }

	inline ptr list_star(ptr a, ptr b) { return cons(a, b); }

	template <typename ...Args>
	inline ptr list_star(ptr a, Args &&...args)
	{ return cons(a, list_star(std::forward<Args>(args)...)); }

	ptr copy_es_immutable(ptr a);
	ptr for_each_pair(ptr a_, std::function<void (ptr)> const &f);
	// }}}
	
	// functions that do something {{{1
	inline bool is_proper_list(ptr a)
	{
		while (not is_nil(a))
		{
			if (not is_pair(a))
				return false;

			a = cdr(a);
		}
		return true;
	}

	inline int proper_list_length(ptr a)
	{
		int c = 0;

		while (not is_nil(a))
		{
			if (not is_pair(a))
				return -1;

			a = cdr(a);
			++c;
		}

		return c;
	}

	extern void match_tree(ptr a, ptr b, std::function<void (ptr, ptr)> const &f);
	extern bool pair_equiv(ptr a, ptr b);

	extern ptr reverse(ptr a);
	extern ptr improper_reverse(ptr a, ptr b);
	extern ptr append_reverse(ptr a, ptr b);
	extern ptr unzip(ptr a);
	extern ptr list_tail(ptr a, int i);
	extern ptr encycle(ptr a, int i, int j);

	extern ptr length(ptr a);
	extern ptr deep_mark(ptr a, char m);
	extern bool deep_compare(ptr a_, ptr b_);
	extern bool deep_congruence(ptr a_, ptr b_);
	extern std::string deep_list_repr(ptr a_);
	extern ptr get_list_metric(ptr a);

	struct List_metric { int p, n, a, c; };
	extern List_metric c_get_list_metric(ptr a);
	extern ptr get_list_metric(ptr a);
	// }}}

	// class ListIterator {{{1
	// TODO: this is now a Static<Object> by default. Should this be
	// such? If we want to unify iterables under a common cloak we
	// are going to need iterators for all linear structures available
	// as dynamic Objects.
	class ListIterator: 
		public Static<Object>, public std::iterator<std::forward_iterator_tag, ptr>
	{
		ptr a;

		public:
			virtual void gc(Edict const &cmd) const
				{ cmd(a); }

			ListIterator(ptr a_): a(a_) {}
			ListIterator &operator++() 
			{ 
				a = cdr(a);

				if ((not is_pair(a)) or mark(a) == 1)
					a = &nil;
				else 
					set_mark(a, 1);

				return *this; 
			}

			bool operator==(ListIterator const &other) const { return a == other.a; }
			bool operator!=(ListIterator const &other) const { return a != other.a; }
			ptr operator*() const { return car(a); }
	}; // }}}

	// class List {{{1
	class List: public Static<Object>
	{
		ptr a;

		public:
			virtual void gc(Edict const &cmd) const
				{ cmd(a); }

			List(ptr a_): a(a_) {}
			ListIterator begin() { return ListIterator(a); }
			ListIterator end() { return ListIterator(&nil); }
			~List() { deep_mark(a, 0); }
	}; // }}}

	// inline ptr c[ad]{2,4}r(ptr p) {{{1
	inline ptr caar(ptr p) { return car(car(p)); }
	inline ptr cdar(ptr p) { return cdr(car(p)); }
	inline ptr cadr(ptr p) { return car(cdr(p)); }
	inline ptr cddr(ptr p) { return cdr(cdr(p)); }

	inline ptr caaar(ptr p) { return car(caar(p)); }
	inline ptr cdaar(ptr p) { return cdr(caar(p)); }
	inline ptr cadar(ptr p) { return car(cdar(p)); }
	inline ptr cddar(ptr p) { return cdr(cdar(p)); }
	inline ptr caadr(ptr p) { return car(cadr(p)); }
	inline ptr cdadr(ptr p) { return cdr(cadr(p)); }
	inline ptr caddr(ptr p) { return car(cddr(p)); }
	inline ptr cdddr(ptr p) { return cdr(cddr(p)); }

	inline ptr caaaar(ptr p) { return car(caaar(p)); }
	inline ptr cdaaar(ptr p) { return cdr(caaar(p)); }
	inline ptr cadaar(ptr p) { return car(cdaar(p)); }
	inline ptr cddaar(ptr p) { return cdr(cdaar(p)); }
	inline ptr caadar(ptr p) { return car(cadar(p)); }
	inline ptr cdadar(ptr p) { return cdr(cadar(p)); }
	inline ptr caddar(ptr p) { return car(cddar(p)); }
	inline ptr cdddar(ptr p) { return cdr(cddar(p)); }
	inline ptr caaadr(ptr p) { return car(caadr(p)); }
	inline ptr cdaadr(ptr p) { return cdr(caadr(p)); }
	inline ptr cadadr(ptr p) { return car(cdadr(p)); }
	inline ptr cddadr(ptr p) { return cdr(cdadr(p)); }
	inline ptr caaddr(ptr p) { return car(caddr(p)); }
	inline ptr cdaddr(ptr p) { return cdr(caddr(p)); }
	inline ptr cadddr(ptr p) { return car(cdddr(p)); }
	inline ptr cddddr(ptr p) { return cdr(cdddr(p)); }
	// }}}
}

// vim:sw=4:ts=4:tw=72:fdm=marker
