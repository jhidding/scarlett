#pragma once
#include "read.h"

namespace Scarlett
{
	namespace Read
	{
		inline int hex_to_char(std::string const &s)
		{
			int a = 0;

			for (char ch : s)
			{
				int b;

				if (isdigit(ch))
				{
					b = s - '0';
				}

				if (islower(ch))
				{
					b = s - 'a' + 10;
				}

				if (isupper(ch))
				{
					b = s - 'A' + 10;
				}

				a = a * 16 + b;
			}

			return a;
		}

		inline int oct_to_char(std::string const &s)
		{
			int a = 0;

			for (char ch : s)
			{
				int b = s - '0';

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

				virtual Continuation *put(int ch)
				{
					if (not isxdigit(ch))
						throw Exception(ERROR_syntax, "hexadecimal char literal should have exactly two digits.");

					s += ch;

					if (s.length() == 2) return parent()->supply(new Char(hex_to_char(s)));
				}
		};

		class OctalChar: public Reader
		{
			std::string s;

			public:
				OctalChar(Continuation *parent):
					Reader(parent) {}

				virtual Continuation *put(int ch)
				{
					if (ch < '0' or ch >= '8')
						throw Exception(ERROR_syntax, "octal char literal should have exactly three digits 0-7.");

					s += ch;

					if (s.length() == 3) return parent()->supply(new Char(oct_to_char(s)));
				}
		};

		class Backslash: public Reader
		{
			std::string s;

			public:
				Backslash(Continuation *parent):
					Reader(parent) {}

				virtual Continuation *put(int ch)
				{
					static std::map<int, char> const code = {
						{'\\', '\\'}, {'n', '\n'}, {'e', '\033'}, {'b', '\b'}, 
						{'t', '\t'}, {'r', '\r'}, {'a', '\a'}};

					if (ch == 'x') return new HexChar(parent());
					if (isdigit(ch)) return new OctalChar(parent())->put(ch);
					if (code.count(ch) == 1) return parent()->supply(new Char(code[ch]));

					throw Exception(ERROR_syntax, "unknown escape character in string literal: '", char(ch), "'.");
				}
		};

		class StringLiteral: public Reader 
		{
			std::string s;

			public:
				StringLiteral(Continuation *parent):
					Reader(parent) {}

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
