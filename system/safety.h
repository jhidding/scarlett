#pragma once
#include "../object.h"
#include "../pair.h"
#include "../exception.h"
#include "../misc/format.h"
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
			virtual std::string description() const = 0;
			virtual bool operator()(ptr a) const = 0;
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

	inline void assert_that(std::string const &, ptr) {}

	template <typename ...Rest>
	inline void assert_that(
			std::string const &place, ptr a, 
			Assertion const &pred, Rest &&...rest)
	{
		if (not pred(a))
			throw Exception(ERROR, "failed test [", pred.description(), "] in ", place);

		assert_that(place, a, std::forward<Rest>(rest)...);
	}
}

