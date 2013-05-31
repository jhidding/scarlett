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
#include <memory>
#include <string>
#include <map>
#include "../object.h"
#include "environment.h"
#include "combiners.h"

/*
 * All global instances of some class will
 * end up in this map, so we can reach them
 * from a central place.
 */

namespace Scarlett
{
	template <typename T>
	class Global: public Static<T>
	{
		typedef std::map<std::string, T *> tmap;
		static std::unique_ptr<tmap> _dir;

		public:
			static tmap &dir() 
			{
				if (not _dir)
					_dir = std::unique_ptr<tmap>(new tmap);

				return *_dir; 
			}

			template <typename ...Args>
			Global(std::string const &name,
				Args &&...args):
				Static<T>(std::forward<Args>(args)...)
			{
				dir()[name] = this;
			}
	};

	class C_module: public Static<Environment>
	{
		public:
			template <typename T>
			C_module &operator<<(std::pair<std::string, T> const &kv)
			{
				define(kv.first, new T(kv.second));
				return *this;
			}
	};

	inline void load_global_env(Environment &env)
	{
		for (auto &kv : Global<C_applicative>::dir())
			env.define(new Symbol(kv.first), kv.second);

		for (auto &kv : Global<C_operative>::dir())
			env.define(new Symbol(kv.first), kv.second);
	}

	template <typename T>
	std::unique_ptr<std::map<std::string, T *>> Scarlett::Global<T>::_dir;
}

