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
#include "misc/autostatic.h"

namespace Scarlett
{
	typedef std::set<std::string> Caps;

	Caps caps_cat()
	{
		return Caps();
	}

	template <typename ...Args>
	Caps caps_cat(Caps::value_type const &a, Args &&...args);

	template <typename ...Args>
	Caps caps_cat(Caps const &A, Args &&...args)
	{
		Caps B = caps_cat(std::forward<Args>(args)...);
		B.insert(A.begin(), A.end());
		return B;
	}

	template <typename ...Args>
	Caps caps_cat(Caps::value_type const &a, Args &&...args)
	{
		Caps B = caps_cat(std::forward<Args>(args)...);
		B.insert(a);
		return B;
	}

	class Iterable: public Interface
	{
		static Autostatic<Caps> _caps;

		public:
			static Caps const &caps()
			{
				return _caps([] () { return Caps({"iterable"}); });
			}

			virtual ptr head() const = 0;
			virtual ptr tail() const = 0;
	};

	class Number: public Interface
	{
		static Autostatic<Caps> _caps;

		public:
			static Caps const &caps()
			{
				return _caps([] () { return Caps({"number"}); });
			}

			virtual ptr operator+(ptr) const = 0;
			virtual ptr operator-(ptr) const = 0;
			virtual ptr operator*(ptr) const = 0;
			virtual ptr operator/(ptr) const = 0;
	};

	template <typename ...Args>
	class Scarljet: public Args...
	{
		static Autostatic<Caps> _caps;

		public:
			static Caps const &caps()
			{
				return _caps([] () { return concat(Args::caps()...); });
			}

			virtual bool implements(Caps::value_type const &feat)
			{
				return caps().count(feat) > 0;
			}
	};
}

