#ifdef UNITTEST

#include <iostream>
#include <map>
#include <complex>
#include <iterator>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include "../object.h"
#include "../pair.h"
#include "../numeric/rational.h"
#include <limits>

#include "../unittest/test.h"
#include "jregex.h"

using namespace Misc;
using namespace Scarlett;


struct Regex_code
{
	size_t 			nsub;
	std::string 	expr;
	std::function<ptr (Regex_match const &)> handle;
};

int string_to_integer(std::string const &expr)
{
	std::istringstream ss(expr);
	int a;
	ss >> a;
	return a;
}

double string_to_double(std::string const &expr)
{
	std::istringstream ss(expr);
	double a;
	ss >> a;
	return a;
}

inline ptr match_to_infnan(Regex_match const &m, unsigned a)
{
	if (m[a+1] == "inf")
		return new Atom<double>(
			(m[a] == "-" ? -1 : 1) *
			std::numeric_limits<double>::infinity());

	else
		return new Atom<double>(
			std::numeric_limits<double>::quiet_NaN());
}

inline ptr match_to_real(Regex_match const &m, unsigned a, int q = 0)
{
	int sign = (m[a] == "-" ? -1 : +1),
		int_part = (m[a+1].empty() ? q : string_to_integer(m[a+1]));

	if (m[a+2].empty() and m[a+3].empty())
		return new Atom<int>(sign * int_part);

	double real = string_to_double(m[a] + m[a+1] + m[a+2] + m[a+3]);
	return new Atom<double>(real);
}

ptr make_rectangular(ptr a, ptr b)
{
	return list(":make-rectangular:"_s, a, b);
}

ptr make_polar(ptr a, ptr b)
{
	return list(":make-polar:"_s, a, b);
}

std::map<std::string, Regex_code> numeric_expression = {
/*	{"integer", 	//  1 - integer
		{ 1, "^[\\+-]?[0-9]+$",
		[] (Regex_match const &m)
		{
			int b = string_to_integer(m[0]);
			return new Atom<int>(b);
		}}}, */

	{"rational", 	//  4 - rational (Sign Num Dem)
		{ 4, "^([\\+-])?([0-9]+)/([0-9]+)$",
		[] (Regex_match const &m)
		{
			int sign = (m[1] == "-" ? -1 : +1),
				a = string_to_integer(m[2]),
				b = string_to_integer(m[3]);

			return new Atom<Numeric::rational>(Numeric::rational(sign * a, b));
		}}},

	{"real",		//  8 - real (Sign i f exp (exp_i) | Sign inf/nan)
		{ 8, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$"
			 "|^([\\+-])(inf|nan)\\.0$",
		[] (Regex_match const &m)
		{
			if (not m[7].empty()) return match_to_infnan(m, 6);
			else return match_to_real(m, 1);
		}}},

/*	{"imag",		//  8 - imag (Sign i f exp (exp_i) | Sign inf/nan)
		{ 8, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
			 "|^([\\+-])(inf|nan)\\.0i$",
		[] (Regex_match const &m)
		{
			ptr p = (m[7].empty() ? match_to_real(m, 1, 1) : match_to_infnan(m, 6));
			return make_rectangular(new Atom<double>(0.0), p);
		}}}, */


	{"complex",		// 17 - complex (
					//	real (Rs Ri Rf Rexp (Rexp_i) | Rs inf/nan)
					//	imag (Is Ii If Iexp (Iexp_i) | Is inf/nan))
		{17, "^(([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			 "|^([\\+-])(inf|nan)\\.0)"
			 "(([\\+-])([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
			 "|([\\+-])(inf|nan)\\.0i)$",
		[] (Regex_match const &m)
		{
			ptr p = (m[8].empty() ? match_to_real(m, 2) : match_to_infnan(m, 7));
			ptr q = (m[16].empty() ? match_to_real(m, 10, 1) : match_to_infnan(m, 15));
			return make_rectangular(p, q);
		}}},

	{"polar", 		// 11 - polar (As Ai Af Aexp (Aexp_i) Fs Fi Ff Fexp (Fexp_i))
		{11, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			 "@([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$",
		[] (Regex_match const &m)
		{
			ptr p = match_to_real(m, 1);
			ptr q = match_to_real(m, 6);
			return make_polar(p, q);
		}}}};

class Readlines
{
	std::istream &in;

	public:
		typedef std::istream_iterator<std::string> iterator;

		Readlines(std::istream &in_):
			in(in_) {}

		iterator begin() { return iterator(in); }
		iterator end() { return iterator(); }
};

Test::Unit Regex_numerical_test(
	"0000 - regex tester",
	"this application contains some regexes that need to work"
	"correctly. feel free to enter some numbers.",
	[] ()
{
	std::vector<std::string> lines = {
		"42", "42.", ".6180339887498949",
		"123.45e23@3.141592",
		"-78.7+11.9i",
		"-inf.0",
		"+inf.0i",
		"-i",
		"898.989e-2",
		"65/123"};

	std::map<std::string, Regex> E;
	for (auto kv : numeric_expression)
	{
		try 
		{
			E[kv.first] = Regex(kv.second.expr, kv.second.nsub);
		}
		catch (std::string const &msg)
		{
			std::cerr 
				<< "regex for " << kv.first << std::endl
				<< kv.second.expr << std::endl 
				<< "gave: " << msg << std::endl;
		}
	}

	for (auto line : lines)
	{
		for (auto kv : E)
		{
			auto match = kv.second(line);

			if (match.is_match())
			{
				ptr a = numeric_expression[kv.first].handle(match);
				std::cout << line << " -> [" << kv.first 
					<< "] -> " << a->repr() << std::endl;
			}
		}
	}

	return true;
});

#endif

// vim:sw=4:ts=4:tw=72
