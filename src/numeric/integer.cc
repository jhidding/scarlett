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

