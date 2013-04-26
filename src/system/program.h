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

