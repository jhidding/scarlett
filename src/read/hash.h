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
#include "comment.h"
#include "char.h"
#include "vector.h"
#include "../numeric/numeric.h"

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

				std::string state() const { return Misc::format("special-literal: `", s, "'"); }

				Continuation *put(int ch)
				{
					static std::map<std::string, std::function<ptr ()>> const lookup = {
						{"t", [] () { return new Boolean(true); }},
						{"f", [] () { return new Boolean(false); }},
						{"nil", [] () { return &nil; }},
						{"inert", [] () { return &inert; }},
						{"ignore", [] () { return &ignore; }},
						{"infinity", [] () { return &infinity; }}};

					if (isspace(ch) or ch == ')' or ch == '(' or ch == '[' or ch == ']')
					{
						auto i = lookup.find(s);
						if (i == lookup.end())
							throw Exception(ERROR_syntax, "unknown hash special: #", s);

						return cast_ptr<Reader>(parent()->supply((i->second)()))->put(ch);
					}

					s += ch;

					return this;
				}
		};

		class Hash: public Reader
		{
			public:
				Hash(Continuation *parent_):
					Reader(parent_) {}

				std::string state() const { return "hash"; }

				Continuation *put(int ch)
				{
					if (ch == '|')
						return new BlockComment(parent());

					if (ch == '\\')
						return new CharLiteral(parent());

					if (ch == '(')
						return new VectorLiteral(parent());

					if (isspace(ch) or ch == ')' or ch == ']')
						throw Exception(ERROR_syntax, "# cannot be followed by ')' or ' '.");

					return (new SpecialLiteral(parent()))->put(ch);
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
