/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

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
