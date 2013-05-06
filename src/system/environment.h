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

#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <map>

#include "../object.h"
#include "../pair.h"

namespace Scarlett
{
	class Environment: public Object
	{
		ptr 						parent_list;
		std::map<std::string, ptr> 	bindings;

		public:
			void gc(Edict const &cmd) const
			{ 
				cmd(parent_list);
				for (auto kv : bindings) cmd(kv.second);
			}

			bool is_mutable() const { return true; }
			std::string type_name() const { return "environment"; }

			Environment(ptr env_lst);
			Environment(std::map<std::string, ptr> const &bindings_, ptr env_lst):
				parent_list(env_lst), bindings(bindings_) {}

			Environment(ptr args, ptr pars, ptr env_lst);

			/*! \brief define a new variable in this environment
			 */
			void define(ptr args, ptr pars);
			void define(std::string const &name, ptr value);

			/*! \brief method looking up a symbol in the environment.
			 *
			 * The environment is checked for the symbol. If it doesn't
			 * appear, the list of parent-environments is sequentially
			 * checked for the symbol. The first match found is
			 * returned. Since every function call creates a new
			 * environment this will eat at the stack if code is
			 * (non-tail) recursive. If the symbol is not found, an
			 * exception is thrown.
			 */
			ptr look_up(std::string const &name);
			ptr look_up(ptr q);

			/*! \brief print out the content of the enviroment. for
			 * debugging purposes only.
			 */
			void print_map(std::ostream &out);
	};

	inline bool is_environment(ptr a) 
		{ return a->type_name() == "environment"; }
}

// vim:sw=4:ts=4:tw=72
