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
		int l;

		public:
			Multi_map(Continuation *C, Environment *env_, 
					Combiner *app_, ptr src_);
			void gc(Edict const &cmd) const;
			Continuation *supply(ptr a);
			Continuation *apply();
	};
}

