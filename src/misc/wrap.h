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
#include <iterator>
#include <functional>

namespace Misc
{
	template <typename T, typename SrcIter>
	class Wrap_fwd_iterator: public std::iterator<std::forward_iterator_tag, T>
	{
		SrcIter i;
		std::function<T (SrcIter)> func;

		public:
			Wrap_fwd_iterator(SrcIter const &i_, std::function<T (SrcIter)> const &func_):
				i(i_), func(func_) {}

			Wrap_fwd_iterator &operator++()
				{ ++i; return *this; }
			bool operator==(Wrap_fwd_iterator const &o) const
				{ return o.i == i; }
			bool operator!=(Wrap_fwd_iterator const &o) const
				{ return o.i != i; }
			T operator*() const
				{ return func(i); }
	};

	template <typename Func, typename SrcIter>
	inline auto wrap_iter(SrcIter const &i, Func const &func) -> Wrap_fwd_iterator<decltype(func(i)), SrcIter>
	{
		return Wrap_fwd_iterator<decltype(func(i)), SrcIter>(i, func);
	}
}

