#pragma once
#include "continuation.h"

namespace Scarlett
{
	class Guarded_continuation: public Continuation
	{
		ptr entry, exit;

		public:
			Guarded_continuation(Continuation *parent, ptr entry_, ptr exit_):
				Continuation(parent), 
				entry(copy_es_immutable(entry_)),
				exit(copy_es_immutable(exit_))
			{}

			void gc(Edict const &cmd)
			{
				Continuation::gc(cmd);
				cmd(entry); cmd(exit);
			}

			Continuation *abnormal_pass(Continuation *source, ptr args)
			{
			}
	};
}

