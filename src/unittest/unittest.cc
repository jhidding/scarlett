/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#include "test.h"
#include "../object.h"

using namespace Scarlett;
using namespace Test;

std::unique_ptr<std::map<std::string, Test::Unit const *>> Test::Unit::_instances;

Unit::imap &Unit::instances()
{
	if (not _instances)
		_instances = std::unique_ptr<imap>(new imap);

	return *_instances;
}

void Unit::all()
{
	static Scarlett::Log log("test");

	for (auto &kv : instances())
	{
		log.entry(kv.first);

		std::cerr << "[test \033[34;1m" << kv.first << "\033[m]\n";

		bool pass;
		try
		{
			pass = (*kv.second)();
		}
		catch (Scarlett::Exception const &e)
		{
			log.entry("exception thrown.");
			pass = false;
			std::cerr << "failure: " << e << std::endl;

			// Scarlett::Log::print_log_book(std::cerr);
		}

		if (pass)
		{
			std::cerr << "\033[62G[\033[32mpassed\033[m]\n";
		}
		else
		{
			std::cerr << "\033[62G[\033[31mfailed\033[m]\n"
				<< Misc::LongString(kv.second->description(), 72,
						[] () -> std::string { return " | "; })
				<< std::endl;
		}

		Scarlett::GC<Scarlett::Object>::cycle();

		/*std::string s; std::cout << "stop? ";
		std::cin >> s;
		if (s == "ja") break;*/
	}
}

Unit::Unit(	
	std::string const &name_, 
	std::string const &description_,
	std::function<bool ()> const &code_ ):

	_name(name_), 
	_description(description_),
	code(code_)
{
	instances()[_name] = this;
}

Unit::~Unit()
{
	instances().erase(_name);
}

