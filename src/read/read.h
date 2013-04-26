#pragma once
#include "../object.h"
#include "../string.h"
#include "../system/system.h"
#include "../system/program.h"

namespace Scarlett
{
	extern Continuation *read_string(Continuation *C, Environment *env, ptr args);
	extern Continuation *read_input_port(Continuation *C, Environment *env, ptr args);
	extern ptr read_istream(std::istream &in);

	inline ptr operator"" _e(char const *c, size_t s)
	{
		Static<Environment> env(&nil);
		Result r;
		Continuation *C = read_string(&r, &env, list(new String(std::string(c))));
		while (C != &r) C = C->apply();
		return r.result();
	}
}

// vim:sw=4:ts=4:tw=72
