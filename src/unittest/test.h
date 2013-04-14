#pragma once

#include <set>
#include <string>
#include <functional>
#include <map>
#include <memory>
#include "../log/log.h"
#include "../misc/longstring.h"
#include "../exception.h"
#include "../system/system.h"
#include "../object.h"

namespace Scarlett {
namespace Test
{
	inline void assert(bool a, std::string const &txt)
	{
		if (not a)
			throw Scarlett::Exception(Scarlett::ERROR_fail, "failed test: ", txt);
	}

	class Unit
	{
		typedef std::map<std::string, Unit const *> imap;
		static std::unique_ptr<imap> _instances;

		std::string const _name, _description;
		std::function<bool ()> const code;

		public:
			static imap &instances();
			static void all();

			~Unit();
			Unit(
				std::string const &name_, 
				std::string const &description_,
				std::function<bool ()> const &code_ );

			bool operator()() const
			{
				return code();
			}

			std::string const &description() const
			{
				return _description;
			}

			std::string const &name() const
			{
				return _name;
			}
	};
} }

// vim:sw=4:ts=4:tw=72
