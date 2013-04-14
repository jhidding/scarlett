#pragma once
#include "../object.h"
#include "../string.h"
#include "../system/system.h"
#include "../system/program.h"

namespace Scarlett
{
	extern Continuation *read_string(Continuation *C, Environment *env, ptr args);

	inline ptr operator"" _e(char const *c, size_t s)
	{
		Static<Environment> env(&nil);
		Result r;
		Program(read_string(&r, &env, list(new String(std::string(c)))), &r).run();
		return r.result();
	}
}

// vim:sw=4:ts=4:tw=72
