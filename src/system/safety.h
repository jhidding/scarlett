#pragma once
#include "../object.h"
#include "../pair.h"
#include "../exception.h"
#include "environment.h"
#include "../misc/format.h"
#include "../read/read.h"
#include <utility>
#include <string>

namespace Scarlett
{
	template <typename Fn, typename ...Args>
	auto call_safe(std::string const &msg, Fn f, Args &&...args) -> decltype(f(args...)) 
	{
		try 
		{ 
			return f(std::forward<Args>(args)...); 
		}
		catch (Exception const &e)
		{
			throw Exception(ERROR, msg, ": ", e);
		}
		catch (...)
		{
			throw Exception(ERROR, msg);
		}
	}

	class Assertion
	{
		public:
			virtual std::string type_name() { return "assertion"; }
			virtual std::string description() const = 0;
			virtual bool operator()(ptr a) const = 0;
	};

	class Or_assertion: public Assertion
	{
		Assertion const &A;
		Assertion const &B;

		public:
			Or_assertion(Assertion const &A_, Assertion const &B_):
				A(A_), B(B_) {}

			std::string description() const
			{
				return Misc::format(A.description(),
					" or ", B.description());
			}

			bool operator()(ptr a) const
			{
				return A(a) or B(a);
			}
	};

	class And_assertion: public Assertion
	{
		Assertion const &A;
		Assertion const &B;

		public:
			And_assertion(Assertion const &A_, Assertion const &B_):
				A(A_), B(B_) {}

			std::string description() const
			{
				return Misc::format(A.description(),
					" and ", B.description());
			}

			bool operator()(ptr a) const
			{
				return A(a) and B(a);
			}
	};

	inline And_assertion operator&&(Assertion const &A, Assertion const &B)
	{
		return And_assertion(A, B);
	}

	inline Or_assertion operator||(Assertion const &A, Assertion const &B)
	{
		return Or_assertion(A, B);
	}

	class Congruent_with: public Assertion
	{
		ptr pattern;
		public:
			Congruent_with(ptr pattern_):
				pattern(car(pattern_)) {}

			virtual void gc(Edict const &cmd) const
				{ cmd(pattern); }

			virtual std::string description() const
			{
				return Misc::format("Arguments should be congruent with ",
					deep_list_repr(pattern));
			}

			bool operator()(ptr a) const
			{
				return deep_congruence(pattern, a);
			}
	};

	class Has_proper_length: public Assertion
	{
		int expect;
		public:
			Has_proper_length(int expect_): 
				expect(expect_) {}
			std::string description() const 
				{ return Misc::format("proper length ", expect); }
			bool operator()(ptr a) const 
				{ return proper_list_length(a) == expect; }
	};

	class Is_boolean: public Assertion
	{
		public:
			std::string description() const { return "boolean?"; }
			bool operator()(ptr a) const { return is_boolean(a); }
	};

	class Is_symbol: public Assertion
	{
		public:
			std::string description() const { return "symbol?"; }
			bool operator()(ptr a) const { return is_symbol(a); }
	};

	class Is_proper_list: public Assertion
	{
		public:
			std::string description() const { return "proper-list?"; }
			bool operator()(ptr a) const { return is_proper_list(a); }
	};

	class Is_environment: public Assertion
	{
		public:
			std::string description() const { return "environment?"; }
			bool operator()(ptr a) const { return is_environment(a); }
	};

	inline void assert_that(std::string const &, ptr) {}

	template <typename ...Rest>
	inline void assert_that(
			std::string const &place, ptr a, 
			Assertion const &pred, Rest &&...rest)
	{
		if (not pred(a))
			throw Exception(ERROR, "failed test [", pred.description(), "] for [", repr(a), "] in ", place);

		assert_that(place, a, std::forward<Rest>(rest)...);
	}
}

