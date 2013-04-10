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

