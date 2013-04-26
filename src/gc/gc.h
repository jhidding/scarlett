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
