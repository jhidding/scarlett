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

#include "../system/system.h"
#include "../iface.h"

using namespace Scarlett;

int plus(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
bool zero(int a) { return a == 0; }
int min(int a, int b) { return a - b; }
Global<C_applicative> Min("-", Curry<int, int, int>(min));
Global<C_applicative> Mul("*", Curry<int, int, int>(mul));
Global<C_applicative> Zero("zero?", Curry<bool, int>(zero));
Global<C_applicative> Plus("+", Curry<int, int, int>(plus));

