#pragma once
#include "object.h"
#include <vector>
#include <algorithm>

namespace Scarlett
{
	class Vector: public Object, public std::vector<ptr>
	{
		public:
			virtual void gc(Edict const &cmd) const
				{ std::for_each(begin(), end(), cmd); }

			virtual std::string type_name() const 
				{ return "vector"; }

			virtual std::string repr() const
			{
				std::stringstream s;
				s 	<< "[" << type_name() << " | " << this << " | "
					<< " size " << size() << "]";
				return s.str();
			}
	};
}

// vim:sw=4:ts=4:tw=72
