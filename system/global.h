#pragma once
#include <memory>
#include <string>
#include <map>
#include "../object.h"

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

	template <typename T>
	std::unique_ptr<std::map<std::string, T *>> Scarlett::Global<T>::_dir;
}

