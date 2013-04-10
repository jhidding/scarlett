#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		std::map<std::string, std::string> expression = {
			{"integer", 
				"[\\+-]?[0-9]+$"},

			{"rational", 
				"([\\+-])?([0-9]+)/([0-9]+)$"},

			{"real",
				"([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$"
				"|([\\+-])(inf|nan)\\.0$"},

			{"imag",
				"([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
				"|([\\+-])(inf|nan)\\.0i$"},

			{"complex",
				"(([\\+-])?([0-9]*)(\\.[0-9]*)?([eE](?[\\+-]?[0-9]+))?"
					"|([\\+-])(inf|nan)\\.0)"
				"(([\\+-])([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i"
					"|([\\+-])(inf|nan)\\.0i)$"},

			{"polar",
				"([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
				"@([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$"}};

		class Number: public Reader
		{
			std::string s;

			public:
				Number(Continuation *parent):
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
