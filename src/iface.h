#include "object.h"
#include "system/system.h"

namespace Scarlett
{
	template <typename T>
	class Convert
	{
		public:
			static inline T const &from_ptr(ptr a) { return cast_ptr<Atom<T>>(a)->value(); }
			static inline ptr to_ptr(T const &value) { return new Atom<T>(value); }
	};

	template <>
	class Convert<ptr>
	{
		public:
			static inline ptr from_ptr(ptr a) { return a; }
			static inline ptr to_ptr(ptr a) { return a; }
	};

	template <typename Ret, typename Arg, typename ...Rest>
	class bind_1st
	{
		typedef std::function<Ret (Arg, Rest...)> F;

		F 		foo;
		Arg 	a;

		public:
			bind_1st(F const &f, Arg a_): foo(f), a(a_) {}

			inline Ret operator()(Rest &&... r)
			{
				return foo(a, std::forward<Rest>(r)...);
			}
	};

	template <typename Ret, typename ...Args>
	class Curry;

	template <typename Ret>
	class Curry<Ret>
	{
		typedef std::function<Ret ()> F;
		F	foo;

		public:
			Curry(F const &f): foo(f)
			{}

			inline Continuation *operator()(Continuation *C, Environment *env, ptr args)
			{
				return C->supply(Convert<Ret>::to_ptr(foo()));
			}

			inline Ret call(ptr args)
			{
				return foo();
			}
	};

	template <typename Ret, typename Arg, typename ...Rest>
	class Curry<Ret, Arg, Rest...>
	{
		typedef std::function<Ret (Arg, Rest...)> F;
		F	foo;

		public:
			Curry(F const &f): foo(f)
			{}

			inline Continuation *operator()(Continuation *C, Environment *env, ptr args)
			{
				if (sizeof...(Rest) != (proper_list_length(args) - 1))
					throw Exception(ERROR, "wrong number of arguments");

				return C->supply(Convert<Ret>::to_ptr(call(args)));
			}

			inline Ret call(ptr args)
			{
				return Curry<Ret, Rest...>(
					bind_1st<Ret, Arg, Rest...>(
						foo, Convert<Arg>::from_ptr(
							car(args)))).call(cdr(args));
			}
	};
}

// vim:sw=4:ts=4:tw=72
