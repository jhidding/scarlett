#pragma once

namespace Scarlett
{
	class Atom<char> public Atom_base<char>
	{
		public:
			Atom<char>(char ch): Atom_base<char>(ch) {}
			std::string type_name() const { return "char"; }
			std::string repr() const { return Misc::format("\\#", value()); }
	};

	inline bool is_char(ptr a) { return a->type_name() == "char"; }

	typedef Atom<char> Char;

	class Atom<std::string>: public Atom_base<std::string>
	{
		public:
			Atom<std::string>(std::string const &s): 
				Atom_base<std::sting>(s)
			{}

			std::string type_name() const { return "string"; }

			void append(Char *ch)
			{
				ref() += ch->value();
			}

			std::string repr() const
			{
				return Misc::format('"', value(), '"');
			}
	};

	inline bool is_string(ptr a) { return a->type_name() == "string"; }

	typedef Atom<std::string> String;
}

