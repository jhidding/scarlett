#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class Number: public Reader
		{
			std::string s;

			public:
				Number(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a) { return parent(); }

				virtual Continuation *put(int ch)
				{
					if (isspace(ch) or ch == ')')
						return parent()->supply(string_to_number(s));

					s += ch;
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
