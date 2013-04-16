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

