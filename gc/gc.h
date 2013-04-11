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

		public:
			typedef std::function<void (T *)> Edict;

			Log log;
			GC(): log("gc") {}
			void *alloc_object(size_t size);
			void add_to_root(T *q);
			void remove_from_root(T *q);
			void do_cycle();

			static void cycle();
	};

	#include "gc.H"
}

// vim:sw=4:ts=4:tw=72
