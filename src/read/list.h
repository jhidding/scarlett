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

#include "read.H"
#include "string.h"
#include "comment.h"
#include "number.h"
#include "symbol.h"

namespace Scarlett
{
	namespace Read
	{
		class CdrLiteral;

		class Dot: public Reader
		{
			public:
				Dot(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("dot"); }

				virtual Continuation *put(int ch);
		};


		class ListLiteral: public Reader
		{
			ptr rev_lst;
			bool improper;
			bool circular;
			int start;

			public:
				ListLiteral(Continuation *parent, int start_);

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(rev_lst); }

				std::string state() const { return Misc::format("list-literal `", rev_lst->repr(), "'"); }

				virtual void poke(Msg a)
				{
					if (a == Improper)
						improper = true;

					if (a == Circular)
						circular = true;
				}

				virtual Continuation *supply(ptr a)
				{
					rev_lst = cons(a, rev_lst);
					return this;
				}

				virtual Continuation *put(int ch);
		};

		class CdrLiteral: public Reader
		{
			public:
				CdrLiteral(Continuation *parent_):
					Reader(parent_) {}

				std::string state() const { return Misc::format("cdr-literal"); }

				virtual Continuation *supply(ptr a) 
					{ return parent()->supply(a); }

				virtual Continuation *put(int ch);
		};
	}
}

// vim:sw=4:ts=4:tw=72
