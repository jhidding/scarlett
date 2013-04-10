#pragma once
#include "../object.h"

namespace Scarlett
{
	class Input_port: public Object
	{
		std::istream in;

		public:
			Input_port(std::istream &in_):
				in(in_.rdbuf()) {}

			int get() { return in.get(); }
			virtual std::string type_name() const { return "input-port"; }
			virtual bool is_mutable() const { return true; }
	};

	namespace Read
	{
		enum Msg { Improper };

		class Reader: public Continuation
		{
			Input_port *in;

			public:
				Reader(Continuation *parent, Input_port *in_):
					Continuation(parent), in(in_) {}

				Reader(Continuation *parent):
					Continuation(parent), in(cast_ptr<Reader>(parent)->in) {}

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

// vim:sw=4:ts=4:tw=72
