#include "number.h"
#include "../numeric/rational.h"
#include "../numeric/complex.h"
#include "../misc/jregex.h"
#include "../object.h"
#include "../pair.h"

#include <algorithm>
#include <sstream>
#include <limits>
#include <functional>
#include <map>
#include <string>

using namespace Scarlett;

inline int string_to_integer(std::string const &expr)
{
	std::istringstream ss(expr);
	int a;
	ss >> a;
	return a;
}

inline double string_to_double(std::string const &expr)
{
	std::istringstream ss(expr);
	double a;
	ss >> a;
	return a;
}

inline ptr match_to_infnan(Misc::Regex_match const &m, unsigned a)
{
	if (m[a+1] == "inf")
		return new Atom<double>(
			(m[a] == "-" ? -1 : 1) *
			std::numeric_limits<double>::infinity());

	else
		return new Atom<double>(
			std::numeric_limits<double>::quiet_NaN());
}

inline ptr match_to_real(Misc::Regex_match const &m, unsigned a, int q = 0)
{
	int sign = (m[a] == "-" ? -1 : +1),
		int_part = (m[a+1].empty() ? q : string_to_integer(m[a+1]));

	if (m[a+2].empty() and m[a+3].empty())
		return new Atom<int>(sign * int_part);

	double real = string_to_double(m[a] + m[a+1] + m[a+2] + m[a+3]);
	return new Atom<double>(real);
}

struct Regex_code
{
	typedef std::function<ptr (Misc::Regex_match const &)> Handle;

	Misc::Regex	expr;
	Handle  	handle;
};

std::map<std::string, Regex_code> const numeric_regexi = {
	{"rational", {	//  4 - rational (Sign Num Dem)
		Misc::Regex("^([\\+-])?([0-9]+)/([0-9]+)$", 4),

		[] (Misc::Regex_match const &m)
		{
			int sign = (m[1] == "-" ? -1 : +1),
				a = string_to_integer(m[2]),
				b = string_to_integer(m[3]);

			return new Atom<Numeric::rational>(Numeric::rational(sign * a, b));
		}}},

	{"real", {	//  8 - real (Sign i f exp (exp_i) | Sign inf/nan)
		Misc::Regex("^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$"
			"|^([\\+-])(inf|nan)\\.0$", 8),

		[] (Misc::Regex_match const &m)
		{
			if (not m[7].empty()) return match_to_infnan(m, 6);
			else return match_to_real(m, 1);
		}}},

	{"complex", {	// 17 - complex (
			//	real (Rs Ri Rf Rexp (Rexp_i) | Rs inf/nan)
			//	imag (Is Ii If Iexp (Iexp_i) | Is inf/nan))
		Misc::Regex("^(([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			"|^([\\+-])(inf|nan)\\.0)"
			"(([\\+-])([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
			"|([\\+-])(inf|nan)\\.0i)$", 17),

		[] (Misc::Regex_match const &m)
		{
			ptr p = (m[8].empty() ? match_to_real(m, 2) : match_to_infnan(m, 7));
			ptr q = (m[16].empty() ? match_to_real(m, 10, 1) : match_to_infnan(m, 15));
			return make_rectangular(p, q);
		}}},

	{"polar", {	// 11 - polar (As Ai Af Aexp (Aexp_i) Fs Fi Ff Fexp (Fexp_i))
		Misc::Regex("^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			"@([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$", 11),

		[] (Misc::Regex_match const &m)
		{
			ptr p = match_to_real(m, 1);
			ptr q = match_to_real(m, 6);
			return make_polar(p, q);
		}}}};

ptr Scarlett::Read::string_to_number(std::string const &s)
{
	for (auto &kv : numeric_regexi)
	{
		Misc::Regex_match m = kv.second.expr(s);
		if (m.is_match())
			return kv.second.handle(m);
	}

	throw Exception(ERROR_syntax, "couldn't parse as a number: ", s);
}

