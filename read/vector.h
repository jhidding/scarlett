#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		class VectorLiteral: public Reader
		{
			public:
				VectorLiteral(Continuation *C):
					Reader(C) {}

				Continuation *put(int ch)
				{
					throw Exception(ERROR_ni, "vector literals are not yet implemented");
				}
		};
	}
}

