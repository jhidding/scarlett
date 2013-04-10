#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class SpecialLiteral: public Reader
		{
			std::string s;

			public:
				SpecialLiteral(Continuation *parent_):
					Reader(parent_) {}

				Continuation *put(int ch)
				{
					static std::map<std::string, std::function<ptr ()>> const lookup = {
						{"t", [] () { return new Boolean(true); }},
						{"f", [] () { return new Boolean(false); }},
						{"nil", [] () { return &nil; }},
						{"inert", [] () { return &inert; }},
						{"ignore", [] () { return &ignore; }}};

					if (isspace(ch) or ch == ')' or ch == '(')
					{
						if (lookup.count(s) == 0)
							throw Exception(ERROR_syntax, "unknown hash special: #", s);

						return parent()->supply(lookup[s]())->put(ch);
					}

					s += ch;
				}
		};

		class Hash: public Reader
		{
			Hash(Continuation *parent_):
				Reader(parent_) {}

			Continuation *put(int ch)
			{
				if (ch == '|')
					return new BlockComment(parent());

				if (ch == '\\')
					return new CharLiteral(parent());

				if (ch == '(')
					return new VectorLiteral(parent());

				if (isspace(ch) or ch == ')')
					throw Exception(ERROR_syntax, "# cannot be followed by ')' or ' '.");

				return new SpecialLiteral(parent());
			}
		};
	}
}

// vim:sw=4:ts=4:tw=72
