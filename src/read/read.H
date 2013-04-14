#pragma once

#include "../system/system.h"
#include "../ports/ports.h"
#include "../misc/format.h"

#include <ctype.h>

namespace Scarlett
{
	namespace Read
	{
		enum Msg { Improper, Circular };

		class Reader: public Continuation
		{
			Input_port *in;

			public:
				Reader(Continuation *parent, Input_port *in_):
					Continuation(parent), in(in_) {}

				Reader(Continuation *parent):
					Continuation(parent), in(cast_ptr<Reader>(parent)->in) {}

				std::string state() const { return "reader"; }

				virtual void gc(Edict const &cmd) const
					{ Continuation::gc(cmd); cmd(in); }

				virtual void poke(Msg a) {}

				virtual Continuation *put(int ch) = 0;

				virtual Continuation *supply(ptr a)
					{ return parent(); }

				virtual Continuation *apply()
					{ return put(in->get()); }
		};
	}
}

