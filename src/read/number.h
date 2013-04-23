#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		extern ptr string_to_number(std::string const &s);

		class Number: public Reader
		{
			std::string s;

			public:
				Number(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("number: `", s, "'"); }

				virtual Continuation *put(int ch)
				{
					if (isspace(ch) or ch == ')' or ch == '(' or ch == '[' or ch == ']')
						return cast_ptr<Reader>(parent()->supply(string_to_number(s)))->put(ch);

					s += ch;
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
