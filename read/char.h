#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		class CharLiteral: public Reader
		{
			public:
				CharLiteral(Continuation *parent_):
					Reader(parent_) {}

				virtual Continuation *put(int ch)
				{
					throw Exception(ERROR_ni, "character literals not yet implemented.");
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
