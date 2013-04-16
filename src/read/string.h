#pragma once
#include "read.H"

namespace Scarlett
{
	namespace Read
	{
		inline int hex_to_char(std::string const &s)
		{
			int a = 0;

			for (char ch : s)
			{
				int b = -1;

				if (isdigit(ch))
				{
					b = ch - '0';
				}

				if (islower(ch))
				{
					b = ch - 'a' + 10;
				}

				if (isupper(ch))
				{
					b = ch - 'A' + 10;
				}

				if (b == -1)
					throw Exception(ERROR, "encountered non-hex char in hex-code.");

				a = a * 16 + b;
			}

			return a;
		}

		inline int oct_to_char(std::string const &s)
		{
			int a = 0;

			for (char ch : s)
			{
				int b = ch - '0';

				a = a * 8 + b;
			}

			return a;
		}

		class HexChar: public Reader
		{
			std::string s;

			public:
				HexChar(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("hexchar-literal `", s, "'"); }

				virtual Continuation *put(int ch)
				{
					if (not isxdigit(ch))
						throw Exception(ERROR_syntax, "hexadecimal char literal should have exactly two digits.");

					s += ch;

					if (s.length() == 2) return parent()->supply(new Char(hex_to_char(s)));

					return this;
				}
		};

		class OctalChar: public Reader
		{
			std::string s;

			public:
				OctalChar(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("octchar-literal `", s, "'"); }

				virtual Continuation *put(int ch)
				{
					if (ch < '0' or ch >= '8')
						throw Exception(ERROR_syntax, "octal char literal should have exactly three digits 0-7.");

					s += ch;

					if (s.length() == 3) return parent()->supply(new Char(oct_to_char(s)));

					return this;
				}
		};

		class Backslash: public Reader
		{
			public:
				Backslash(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return "backslash-literal"; }

				virtual Continuation *put(int ch)
				{
					static std::map<int, char> const code = {
						{'\\', '\\'}, {'n', '\n'}, {'e', '\033'}, {'b', '\b'}, 
						{'t', '\t'}, {'r', '\r'}, {'a', '\a'}};

					if (ch == 'x') return new HexChar(parent());
					if (isdigit(ch)) return (new OctalChar(parent()))->put(ch);

					auto i = code.find(ch);
					if (i != code.end()) return parent()->supply(new Char(i->second));

					throw Exception(ERROR_syntax, "unknown escape character in string literal: '", char(ch), "'.");
				}
		};

		class StringLiteral: public Reader 
		{
			std::string s;

			public:
				StringLiteral(Continuation *parent):
					Reader(parent) {}

				std::string state() const { return Misc::format("string-literal: `", s, "'"); }

				virtual Continuation *supply(ptr a)
				{
					if (is_char(a))
						s += cast_ptr<Char>(a)->value();
					else
						throw Exception(ERROR, "a string reader can only receive Chars");

					return this;
				}

				virtual Continuation *put(int ch)
				{
					if (ch == '\\') return new Backslash(this);
					if (ch == '"') return parent()->supply(new String(s));

					s += ch;
					return this;
				}
		};
	}
}

// vim:sw=4:ts=4:tw=72
