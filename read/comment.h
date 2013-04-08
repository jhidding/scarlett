#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class Comment: public Reader
		{
			public:
				Comment(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a) { return parent(); }

				virtual Continuation *put(int ch)
				{
					if (ch == '\n') return parent();
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
