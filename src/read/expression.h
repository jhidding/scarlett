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
#include "hash.h"
#include "string.h"
#include "list.h"
#include "comment.h"
#include "number.h"
#include "symbol.h"
#include "sign.h"

#include "../object.h"
#include "../pair.h"

namespace Scarlett
{
	namespace Read
	{
		class Expression: public Reader
		{
			ptr rev_lst;

			public:
				Expression(Continuation *parent, Input_port *in):
					Reader(parent, in), rev_lst(&nil) {}

				Expression(Continuation *parent):
					Reader(parent), rev_lst(&nil) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(rev_lst); }

				std::string state() const { return Misc::format("expression: `", rev_lst->repr(), "'"); }

				virtual Continuation *supply(ptr a)
				{
					rev_lst = cons(a, rev_lst);
					return this;
				}

				virtual Continuation *put(int ch)
				{
					if (ch == '#') return new Hash(this);
					if (ch == '"') return new StringLiteral(this);
					if (ch == '(' or ch == '[') 
						return new ListLiteral(this, ch);
					if (ch == ';') return new Comment(this);
					if (ch == '+' or ch == '-') return new Sign(this, ch);

					if (isdigit(ch) or ch == '.') 
						return (new Number(this))->put(ch);
					if (isspace(ch)) return this;

					if (ch == EOF) return parent()->supply(reverse(rev_lst));

					if (ch == ')' or ch == ']') 
						throw Exception(ERROR_syntax, "encountered ')' in top level.");

					return (new SymbolLiteral(this))->put(ch);
				}
		};
	}
}

