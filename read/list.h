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

			public:
				ListLiteral(Continuation *parent):
					Reader(parent), rev_lst(&nil), improper(false) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(rev_lst); }

				std::string state() const { return Misc::format("list-literal `", rev_lst->repr(), "'"); }

				virtual void poke(Msg a)
				{
					if (a == Improper)
						improper = true;
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
