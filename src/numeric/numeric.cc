#include "inf.h"
#include "numeric.h"

using namespace Scarlett;

Static<Special> Scarlett::infinity(Special("infinity"));

int Scarlett::gcd(int a, int b)
{
	while (a != 0 and b != 0)
	{
		if (a > b)
			a %= b;
		else
			b %= a;
	}

	if (a == 0) return b;
	if (b == 0) return a;

	throw Exception(ERROR, "gcd algorithm seems to be failing...");
}

int Scarlett::lcm(int a, int b)
{
	return a * b / gcd(a, b);
}

int Scarlett::max(int a, int b)
{
	return (a > b ? a : b);
}

bool Scarlett::is_zero(ptr a)
{
	return cast_ptr<Atom<int>>(a)->value() == 0;
}

