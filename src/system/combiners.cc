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

#include "system.h"
#include "../iface.h"

using namespace Scarlett;

Global<C_applicative> Scarlett::Is_applicative("applicative?", Curry<bool, ptr>(is_applicative));
Global<C_applicative> Scarlett::Is_operative("operative?", Curry<bool, ptr>(is_operative));
Global<C_applicative> Is_combiner("combiner?", Curry<bool, ptr>(is_combiner));

