#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class Comment: public Reader
		{
			public:
				Comment(Continuation *parent_):
					Reader(parent_) {}

				virtual Continuation *supply(ptr a) { return parent(); }

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
