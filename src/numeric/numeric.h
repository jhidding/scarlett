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
#include "../object.h"

namespace Scarlett
{
	extern int gcd(int a, int b);
	extern int lcm(int a, int b);
	extern int max(int a, int b);
	extern bool is_zero(ptr a);

	/*! Interface common to all numeric values
	 
	class Number_interface
	{
		public:
			virtual ptr add(ptr a) const = 0;
			virtual ptr sub(ptr a) const = 0;
			virtual ptr mul(ptr a) const = 0;
			virtual ptr div(ptr a) const = 0;
			virtual ptr mod(ptr a) const = 0;
			virtual bool lt(ptr a) const = 0;
			virtual bool gt(ptr a) const = 0;

			ptr _add_(ptr a) const { return add(a); }
			ptr _sub_(ptr a) const { return sub(a); }
			ptr _mul_(ptr a) const { return mul(a); }
			ptr _div_(ptr a) const { return div(a); }
			ptr _mod_(ptr a) const { return mod(a); }
			bool _lt_(ptr a) const { return lt(a); }
			bool _gt_(ptr a) const { return gt(a); }
	};

	template <typename T>
	class Numeric: public Atom_base<T>, public Number_interface
	{
		public:
			virtual ptr add(ptr a) const;
			virtual ptr sub(ptr a) const;
			virtual ptr mul(ptr a) const;
			virtual ptr div(ptr a) const;
			virtual ptr mod(ptr a) const;
			virtual bool lt(ptr a) const;
			virtual bool gt(ptr a) const;
	};

	template <typename T> virtual ptr  Numeric<T>::add(ptr a) const
	{}

	template <typename T> virtual ptr  Numeric<T>::sub(ptr a) const
	{}

	template <typename T> virtual ptr  Numeric<T>::mul(ptr a) const
	{}

	template <typename T> virtual ptr  Numeric<T>::div(ptr a) const
	{}

	template <typename T> virtual ptr  Numeric<T>::mod(ptr a) const
	{}

	template <typename T> virtual bool Numeric<T>::lt(ptr a)  const
	{}

	template <typename T> virtual bool Numeric<T>::gt(ptr a)  const
	{}

	template <>
	Atom<int>: public Numeric<int>
	{
	};

	template <>
	Atom<double>: public Numeric<double>
	{
	};

	template <>
	Atom<std::complex<double>>: public Numeric<std::complex<double>>
	{
	};*/
}

