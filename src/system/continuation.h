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
#include "../object.h"

namespace Scarlett
{
	/*! \brief object that contains the continuation.
	 *
	 * The continuation is a function, when called with the result of
	 * the current evaluation, runs the rest of the program. It contains
	 * a function that is being called, and where to pass the result,
	 * being the previous (partial) continuation.
	 *
	 * Because C++ does not support tail call optimisation, we have
	 * to achieve this feature on our own. The key thing to realize is
	 * that TCO is replacing imperative programming. We give a variable
	 * some value, do something to it, then decide to give the variable
	 * a new value, and repeat the previous action.
	 *
	 * Our design for the Continuation contains two key methods. One for
	 * supplying arguments. They are saved in the continuation.
	 * Typically when apply is called, these arguments are passed to a
	 * third party, along with the target continuation. The target may
	 * be parent in case of a simple function application, this in the
	 * case of a continued map (more items to be consed onto target), or
	 * a new continuation in case of a sequence. Apply may only be
	 * called by the main controller.
	 */
	class Continuation: public Object
	{
		Continuation *m_parent;

		public:
			Continuation() = default;

			Continuation(Continuation *parent_): 
				m_parent(parent_) {}

			Continuation *parent() const { return m_parent; }
			void gc(Edict const &cmd) const
			{ cmd(m_parent); }

			std::string type_name() const { return "continuation"; }
			virtual std::string state() const { return "abstract"; }

			// this method is called when the continuation is called as
			// a result of a call/cc construct. By default the arguments
			// are just passed through normally, however we may define
			// a specialisation that will provide continuation guarding.
			virtual Continuation *abnormal_pass(Continuation *source, ptr args)
				{ return this->supply(args); }

			// this method is called by a function to return values
			// it should never call any other functions, just store data
			// if any work needs to be done on it, return *this and have
			// the apply method do it for you.
			virtual Continuation *supply(ptr) = 0;

			// this method is called by the cycle controller, if you
			// create a new continuation here, remember this method will
			// be called again, so you'll create a loop. If this is what
			// you want, better call a function with *this as
			// continuation.
			virtual Continuation *apply() = 0;
	};

	inline bool is_continuation(ptr a)
		{ return a->type_name() == "continuation"; }
}

// vim:sw=4:ts=4:tw=72
