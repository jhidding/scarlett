#pragma once
#include "object.h"
#include "misc/format.h"
#include <map>

#include <string>

namespace Scarlett
{
	template <>
	class Atom<char>: public Atom_base<char>
	{
		public:
			Atom<char>(char ch): Atom_base<char>(ch) {}
			std::string type_name() const { return "char"; }
			std::string repr() const { return Misc::format("\\#", value()); }
	};

	typedef Atom<char> Char;

	inline bool is_char(ptr a) { return a->type_name() == "char"; }

	template <>
	class Atom<std::string>: public Atom_base<std::string>
	{
		static std::map<char, std::string> char_repr;

		public:
			Atom<std::string>(std::string const &s): 
				Atom_base<std::string>(s)
			{}

			std::string type_name() const { return "string"; }

			void append(Char *ch)
			{
				ref() += ch->value();
			}

			std::string repr() const;
	};

	typedef Atom<std::string> String;

	inline bool is_string(ptr a) { return a->type_name() == "string"; }
	inline String *operator "" _a(char const *c, size_t s) { return new String(c); }
}

