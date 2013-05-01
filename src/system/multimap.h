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
#include "continuation.h"
#include "../pair.h"

namespace Scarlett
{
	class Multi_map: public Continuation
	{
		Environment *env;
		Combiner *app;
		ptr src, tgt;
		
		List_metric src_metric, tgt_metric;
		bool improper;
		int l;

		public:
			Multi_map(Continuation *C, Environment *env_, 
					Combiner *app_, ptr src_);
			std::string state() const;
			void gc(Edict const &cmd) const;
			Continuation *supply(ptr a);
			Continuation *apply();
	};
}

