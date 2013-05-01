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

namespace Scarlett
{
	namespace Read
	{
		class Comment: public Reader
		{
			public:
				Comment(Continuation *parent_):
					Reader(parent_) {}

				std::string state() const { return "comment"; }

				virtual Continuation *put(int ch)
				{
					if (ch == '\n') return parent();
					return this;
				}
		};

		class BlockComment: public Reader
		{
			enum { hash, vert, other } last;

			public:
				BlockComment(Continuation *parent_):
					Reader(parent_), last(other) {}

				std::string state() const { return "block-comment"; }

				Continuation *put(int ch)
				{
					switch (ch)
					{
						case '#': 
							if (last == vert)
							{
								return parent();
							}

							last = hash;
							return this;

						case '|':
							if (last == hash)
							{
								last = other;
								return new BlockComment(this);
							}

							last = vert;
							return this;

						default:
							return this;
					}
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
