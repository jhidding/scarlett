#ifdef UNITTEST

#include <iostream>
#include <map>
#include <complex>
#include <iterator>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>
#include "jregex.h"

struct Regex_code
{
	size_t 			nsub;
	std::string 	expr;
	std::vector<std::string> args;
};

std::map<std::string, Regex_code> numeric_expression = {
	{"integer", 	//  1 - integer
		{ 1, "^[\\+-]?[0-9]+$",
			{"integer"}}},

	{"rational", 	//  4 - rational (Sign Num Dem)
		{ 4, "^([\\+-])?([0-9]+)/([0-9]+)$",
			{"rational", "sign", "numerator", "denominator"}}},

	{"real",		//  8 - real (Sign i f exp (exp_i) | Sign inf/nan)
		{ 8, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$"
			 "|^([\\+-])(inf|nan)\\.0$",
			 {"real", "sign", "integer", "fraction", "exp", "expint", "inf-sign", "inf/nan"}}},

	{"imag",		//  8 - imag (Sign i f exp (exp_i) | Sign inf/nan)
		{ 8, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
			 "|^([\\+-])(inf|nan)\\.0i$",
			 {"imag", "sign", "integer", "fraction", "exp", "expint", "inf-sign", "inf/nan"}}},

	{"complex",		// 17 - complex (
					//	real (Rs Ri Rf Rexp (Rexp_i) | Rs inf/nan)
					//	imag (Is Ii If Iexp (Iexp_i) | Is inf/nan))
		{17, "^(([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			 "|^([\\+-])(inf|nan)\\.0)"
			 "(([\\+-])([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?i$"
			 "|([\\+-])(inf|nan)\\.0i)$",
			 { "complex", "real-part", "real-sign", "real-integer", "real-fraction", 
			   "real-exp", "real-expint", "real-inf-sign", "real-inf/nan",
			   "imag-part", "imag-sign", "imag-integer", "imag-fraction", 
			   "imag-exp", "imag-expint", "imag-inf-sign", "imag-inf/nan"}}},

	{"polar", 		// 11 - polar (As Ai Af Aexp (Aexp_i) Fs Fi Ff Fexp (Fexp_i))
		{11, "^([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?"
			 "@([\\+-])?([0-9]*)(\\.[0-9]*)?([eE]([\\+-]?[0-9]+))?$",
			 {"polar", "amp-sign", "amp-integer", "amp-fraction", "amp-exp", "amp-expint",
			  "phase-sign", "phase-integer", "phase-fraction", "phase-exp", "phase-expint"}}}};

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

#include "../unittest/test.h"
#include "regex.h"

using namespace Misc;

Test::Unit Regex_numerical_test(
	"0000 - regex tester",
	"this application contains some regexes that need to work"
	"correctly. feel free to enter some numbers.",
	[] ()
{
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

	for (auto line : Readlines(std::cin))
	{
		for (auto kv : E)
		{
			auto match = kv.second(line);
			std::cout 
				<< " + " << kv.first << ": " 
				<< (match.is_match() ? "match" : "no match") 
				<< std::endl;

			if (match.is_match())
			{
				for (unsigned i = 0; i < numeric_expression[kv.first].nsub; ++i)
				{
					std::cout 
						<< " | " << std::setw(20) << numeric_expression[kv.first].args[i] 
						<< ": `" << match[i] << "'" << std::endl;
				}
			}
		}

		std::cout << "=======================================\n";
	}

	return true;
});

#endif

// vim:sw=4:ts=4:tw=72
