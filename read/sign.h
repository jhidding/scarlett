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
					if (isspace(ch) or ch == ')' or ch == '(' or ch == EOF)
						return cast_ptr<Reader>((new SymbolLiteral(parent()))->put(q))->put(ch);

					return cast_ptr<Reader>((new Number(parent()))->put(q))->put(ch);
				}
		};
	}
}

