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

#include "object.h"
#include "system/system.h"

namespace Scarlett
{
	/*! converts generic values to Scarlett pointers and back.
	 *
	 * When interfacing Scarlett with C++, we need to convert general
	 * data to pointers. Data is encapsulated into \link Atom<T>.
	 * However if type T is \link ptr , nothing is done.
	 */
	template <typename T>
	class Convert
	{
		public:
			//! converting from a \link ptr to type T
			static inline T const &from_ptr(ptr a) { return cast_ptr<Atom<T>>(a)->value(); }

			//! constructs an \link Atom<T> from a value of type T
			static inline ptr to_ptr(T const &value) { return new Atom<T>(value); }
	};

	template <>
	class Convert<ptr>
	{
		public:
			static inline ptr from_ptr(ptr a) { return a; }
			static inline ptr to_ptr(ptr a) { return a; }
	};

	/*! stores the first argument of a function
	 *
	 * Stores the first argument to a function to create
	 * a function object that has one less parameter.
	 * @param f some function
	 * @param a argument
	 */
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

	/*! Interfaces C++ functions to Scarlett by curry.
	 *
	 * Has at least one template argument
	 * @param Ret return type
	 * @param Args... parameter types
	 *
	 * The Curry<> object behaves as a sequence of single parameter
	 * functions; every function in the sequence returning another
	 * Curry<> of one parameter less. The call operator is overloaded
	 * to behave like a Scarlett \link CEO: the first argument is the
	 * \link continuation, the second the \link environment.  The third
	 * argument a Scarlett list containing the call arguments. Example:
	 * \code{.cpp}
	 * int fun(double a, std::string const &b) { ... }
	 *
	 * CEO Foo = Curry<int, double, std::string const &>(fun);
	 * \endcode
	 */
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

	/*! Universal object factory
	 *
	 * Makes a function of an object constructor. The result
	 * can be passed to Curry<Type, Args...>
	 * @param Type a class that needs an interface to Scarlett
	 * @param Args... the argument types of the constructor
	 * @see Curry
	 */
	template <typename T, typename ...Args>
	T create(Args &&...args)
	{
		return T(std::forward<Args>(args)...);
	}
}

// vim:sw=4:ts=4:tw=72
