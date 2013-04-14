#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		class SymbolLiteral: public Reader
		{
			std::string s;

			public:
				SymbolLiteral(Continuation *C):
					Reader(C) {}

				std::string state() const { return Misc::format("symbol-literal `", s, "'"); }

				Continuation *put(int ch)
				{
					if (isspace(ch) or ch == '(' or ch == ')' or ch == EOF)
						return cast_ptr<Reader>(parent()->supply(new Symbol(s)))->put(ch);
					else
						s += ch;

					return this;
				}
		};
	}
}

