#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		class Hash: public Reader
		{
			Hash(Reader *parent):
				Reader(parent) {}

			Continuation *supply() { return parent(); }

			Continuation *put(int ch)
			{
				if (isspace(ch) or ch == ')' or ch == '(')
					return parent()->supply(lookup[s]())->put(ch);

				s += ch;
				return this;
			}
		};
	}
}

// vim:sw=4:ts=4:tw=72
