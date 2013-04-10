#pragma once

#include "read.h"
#include "string.h"
#include "comment.h"
#include "number.h"
#include "dot.h"

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

				virtual Continuation *put(int ch)
				{
					if (isspace(ch))
					{
						parent()->poke(Improper);
						return new CdrLiteral(parent());
					}

					if (isnumber(ch))
					{
						return new Number(parent())->put('.')->put(ch);
					}

					throw Exception(ERROR_syntax, 
							"'.' should be followed by digit for a number, "
							"or by white space to make a pair.");
				}
		};

		class List: public Reader
		{
			ptr rev_lst;
			bool improper;

			public:
				List(Continuation *parent):
					Reader(parent), rev_lst(&nil), improper(false) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(rev_lst); }


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

				virtual Continuation *put(int ch)
				{
					if (ch == '#') return new Hash(this);
					if (ch == '"') return new StringLiteral(this);
					if (ch == '(') return new List(this);
					if (ch == ';') return new Comment(this);
					if (isdigit(ch)) return (new Number(this))->put(ch);
					if (isspace(ch)) return this;

					if (ch == '.') return new Dot(this);
					if (ch == ')') 
					{
						if (improper)
							return parent()->supply(append_reverse(cdr(rev_lst), car(rev_lst)));
						else
							return parent()->supply(reverse(rev_lst));
					}

					return new Symbol(this);
				}
		};

		class CdrLiteral: public Reader
		{
			ptr tail;

			public:
				CdrLiteral(Continuation *parent_):
					Reader(parent_) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(tail); }

				virtual Continuation *supply(ptr a) 
					{ return parent()->supply(tail); }

				virtual Continuation *put(int ch)
				{
					if (ch == ')') throw Exception(ERROR_syntax, 
						"didn't expect ')' immediately after '.' ");
					if (ch == '.') throw Exception(ERROR_syntax, 
						"found erronous ' . . '");

					if (ch == '#') return new Hash(parent());
					if (ch == '"') return new String(parent());
					if (ch == '(') return new List(parent());
					if (ch == ';') return new Comment(this);
					if (isdigit(ch)) return (new Number(parent()))->put(ch);
					if (isspace(ch)) return this;

					return new Symbol(parent());
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
