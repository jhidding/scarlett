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

#include "numeric.h"

using namespace Scarlett;

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

