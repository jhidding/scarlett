#pragma once
#include "read.h"
#include "number.h"
#include "list.h"

namespace Scarlett
{
	namespace Read
	{
		class Dot: public Reader
		{
			public:
				Dot(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a) { return parent(); }
				virtual Continuation *put(int ch)
				{
					if (isspace(ch))
					{
						parent()->poke(Improper);
						return new Cdr(parent());
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
	}
}

// vim:sw=4:ts=4:tw=72
