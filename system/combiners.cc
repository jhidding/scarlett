#include "system.h"
#include "../iface.h"

using namespace Scarlett;

Global<C_applicative> Scarlett::Is_applicative("applicative?", Curry<bool, ptr>(is_applicative));
Global<C_applicative> Scarlett::Is_operative("operative?", Curry<bool, ptr>(is_operative));

