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
#include <iterator>
#include "../object.h"
#include "continuation.h"
#include "environment.h"

namespace Scarlett
{
	class Program: public Static<Object>
	{
		Continuation *_begin, *_end, *_cc;

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
				_begin(begin_), _end(end_), _cc(NULL) {}

			virtual void gc(Edict const &cmd) const 
			{ 
				if (_cc != NULL) cmd(_cc);
				cmd(_begin); cmd(_end); 
			}

			iterator begin() const { return iterator(_begin); }
			iterator end() const { return iterator(_end); }

			void run(bool echo = false)
			{
				_cc = _begin;
				while (_cc != _end) 
				{
					if (echo)
						std::cerr << _cc->state() << std::endl;

					GC<Object>::cycle();
					_cc = _cc->apply();
				}
			} 
	};

	class Result: public Static<Continuation>
	{
		ptr _result;

		public:
			Result() {}

			virtual void gc(Edict const &cmd) const { cmd(_result); }

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
}

