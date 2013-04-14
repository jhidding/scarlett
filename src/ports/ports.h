#pragma once
#include "../object.h"
#include "../string.h"
#include <iostream>
#include <sstream>
#include <string>

namespace Scarlett
{
	class Input_port: public Object
	{
		public:
			virtual std::string type_name() const { return "input-port"; }
			virtual bool is_mutable() const { return true; }
			virtual int get() = 0;
	};

	class Stream_input_port: public Input_port
	{
		std::istream in;

		public:
			Stream_input_port(std::istream &in_):
				in(in_.rdbuf()) {}

			int get() { return in.get(); }
	};

	class String_input_port: public Input_port
	{
		std::istringstream in;

		public:
			String_input_port(std::string const &s):
				in(s) {}

			String_input_port(String *s):
				in(s->value()) {}

			int get() { return in.get(); }
	};
}

