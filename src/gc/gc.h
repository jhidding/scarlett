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
#include <functional>
#include <algorithm>
#include <set>

#include "../log/log.h"

namespace Scarlett
{
	template <typename T>
	class GC
	{
		typedef std::set<T *> S;
		S root, white, gray, black;
		size_t i;

		public:
			typedef std::function<void (T *)> Edict;

			Log log;
			GC(): root(), white(), gray(), black(), i(0), log("gc")
			{
				root.clear(); white.clear(); gray.clear(); black.clear();
			}

			void *alloc_object(size_t size);
			void add_to_root(T *q);
			void remove_from_root(T *q);
			void do_cycle(bool);

			static void cycle(bool);
	};

	#include "gc.H"
}

// vim:sw=4:ts=4:tw=72
