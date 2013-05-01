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
#include "number.h"
#include "symbol.h"

namespace Scarlett
{
	namespace Read
	{
		class Sign: public Reader
		{
			int q;

			public:
				Sign(Continuation *C, int q_):
					Reader(C), q(q_) {}

				Continuation *put(int ch)
				{
					if (isspace(ch) or ch == ')' or ch == '(' 
							or ch == ']' or ch == '['
							or ch == EOF)
						return cast_ptr<Reader>((new SymbolLiteral(parent()))->put(q))->put(ch);

					return cast_ptr<Reader>((new Number(parent()))->put(q))->put(ch);
				}
		};
	}
}

