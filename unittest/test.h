#pragma once

#include <set>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include "../log/log.h"
#include "../misc/longstring.h"
#include "../exception.h"
#include "../system/system.h"
#include "../object.h"

namespace Scarlett {
namespace Test
{
	inline void assert(bool a, std::string const &txt)
	{
		if (not a)
			throw Scarlett::Exception(Scarlett::ERROR_fail, "failed test: ", txt);
	}

	class Unit
	{
		typedef std::map<std::string, Unit const *> imap;
		static std::unique_ptr<imap> _instances;

		std::string const _name, _description;
		std::function<bool ()> const code;

		public:
			static imap &instances();
			static void all();

			~Unit();
			Unit(
				std::string const &name_, 
				std::string const &description_,
				std::function<bool ()> const &code_ );

			bool operator()() const
			{
				return code();
			}

			std::string const &description() const
			{
				return _description;
			}

			std::string const &name() const
			{
				return _name;
			}
	};

	class Program: public Static<Object>
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
} }

// vim:sw=4:ts=4:tw=72
