#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class String: public Reader 
		{
			std::string s;

			public:
				String(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a)
				{
					if (is_char(a))
						s += cast_ptr<Char>(a)->value();

					return this;
				}

				virtual Continuation *put(int ch)
				{
					if (ch == '\\') return new Char(this);
					if (ch == '"') return parent()->supply(new String(s));

					s += ch;
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
