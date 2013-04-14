#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		class Ellipsis: public Reader
		{
			public:
				Ellipsis(Continuation *C):
					Reader(C) {}

				std::string state() const { return Misc::format("ellipsis"); }

				Continuation *put(int ch)
				{
					if (ch == '.') return parent();
					throw Exception(ERROR_syntax, "`..' is always followed by a third `.' to create an ellipsis.");
				}
		};
	}
}

