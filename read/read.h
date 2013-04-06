#pragma once
#include "../object.h"

namespace Scarlett
{
	class Input_port: public Object
	{
		std::istream in;

		public:
			Input_port(std::istream &in_):
				in(in_.rdbuf()) {}

			int get() { return in.get(); }
			virtual std::string type_name() const { return "input-port"; }
			virtual bool is_mutable() const { return true; }
	};

	namespace Read
	{
		enum Msg { Improper };

		class Reader: public Continuation
		{
			Input_port *in;

			public:
				Reader(Continuation *parent, Input_port *in_):
					Continuation(parent), in(in_) {}

				Reader(Reader *parent):
					Continuation(parent), in(parent->in) {}

				virtual void gc(Edict const &cmd) const
					{ Continuation::gc(cmd); cmd(in); }

				virtual void poke(Msg a) {}

				virtual Continuation *put(int ch) = 0;

				virtual Continuation *apply()
					{ return put(in->get()); }
		};

		class List: public Reader
		{
			ptr rev_lst;
			bool improper;

			public:
				List(Reader *parent):
					Reader(parent), rev_lst(&nil), improper(false) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(rev_lst); }


				virtual void poke(Msg a)
				{
					if (a == Improper)
						improper = true;
				}

				virtual Continuation *supply(ptr a)
				{
					rev_lst = cons(a, rev_lst);
					return this;
				}

				virtual Continuation *put(int ch)
				{
					if (ch == '#') return new Hash(this);
					if (ch == '"') return new String(this);
					if (ch == '(') return new List(this);
					if (ch == ';') return new Comment(this);
					if (isdigit(ch)) return (new Number(this))->put(ch);
					if (isspace(ch)) return this;

					if (ch == '.') return new Dot(this);
					if (ch == ')') 
					{
						if (improper)
							return parent()->supply(append_reverse(cdr(rev_lst), car(rev_lst)));
						else
							return parent()->supply(reverse(rev_lst));
					}

					return new Symbol(this);
				}
		};

		class Hash: public Reader {};

		class Char: public Reader
		{
		};

		class String: public Reader 
		{
			Scarlett::String *str;

			public:
				String(Reader *parent):
					Reader(parent), str(new Scarlett::String) {}

				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(str); }

				virtual Continuation *supply(ptr a)
				{
					str->append(a);
					return this;
				}

				virtual Continuation *put(int ch)
				{
					if (ch == '\\') return new Char(this);
					if (ch == '"') return parent()->supply(str);

					str->append(ch);
					return this;
				}
		};

		class Comment: public Reader
		{
			public:
				Comment(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a) { return parent(); }

				virtual Continuation *put(int ch)
				{
					if (ch == '\n') return parent();
					return this;
				}
		};

		class Number: public Reader {};

		class Cdr: public Reader
		{
			ptr tail;

			public:
				virtual void gc(Edict const &cmd) 
					{ Reader::gc(cmd); cmd(tail); }

				virtual Continuation *supply(ptr a) 
					{ return parent()->supply(tail); }

				virtual Continuation *put(int ch)
				{
					if (ch == ')') throw Exception(ERROR_syntax, "didn't expeect closing parenthesis.");
					if (ch == '.') throw Exception(ERROR_syntax, "found erronous ' . . '";

					if (ch == '#') return new Hash(parent());
					if (ch == '"') return new String(parent());
					if (ch == '(') return new List(parent());
					if (ch == ';') return new Comment(this);
					if (isdigit(ch)) return (new Number(parent()))->put(ch);
					if (isspace(ch)) return this;

					return new Symbol(parent());
				}
		};

		class Dot: public Reader
		{
			public:
				Dot(Reader *parent):
					Reader(parent) {}

				virtual Continuation *supply(ptr a) { return parent(); }
				virtual Continuation *put(int ch)
				{
					if (isspace(ch))
					{
						parent()->poke(Improper);
						return new Cdr(parent());
					}

					if (isnumber(ch))
					{
						return new Decimal_fraction(parent());
					}
				}
		};
	}
}

