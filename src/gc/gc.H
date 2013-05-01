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

template <typename S>
class Juggle
{
	typedef typename S::value_type T;
	typedef std::function<void (T)> Juggler;

	public:
		static void move(S &A, S &B, T q)
		{
			if (A.erase(q))	B.insert(q);
		}

		static Juggler mover(S &A, S &B)
		{
			return [&A, &B] (T q)
			{
				Juggle::move(A, B, q);
			};
		}

		static void blacken(S &A, S &B, T q)
		{
			if (q == NULL) return;
			q->gc(Juggle::mover(A, B));
		}

		static Juggler blackener(S &A, S &B)
		{
			return [&A, &B] (T q)
			{
				Juggle::blacken(A, B, q);
			};
		}

		static void destroyer(T q)
		{
			delete q;
		}
};

template <typename T>
void *Scarlett::GC<T>::alloc_object(size_t size)
{
	++i;
	void *mem = ::operator new(size);
	log.entry("new object: ", mem);
	gray.insert(reinterpret_cast<T *>(mem));
	return mem;
}

template <typename T>
void Scarlett::GC<T>::add_to_root(T *q)
{
	if (white.count(q) > 0)
	{
		log.entry("object could not be allowed into root: ", q->spec());
	}
	else
	{
		log.entry("added object to root: ", q->spec());
		root.insert(q);
	}
}

template <typename T>
void Scarlett::GC<T>::remove_from_root(T *q)
{
	root.erase(q);
}

template <typename T>
void Scarlett::GC<T>::do_cycle(bool force = false)
{
	if (i < 50 and not force)
	{
		return;
	}

	i = 0;
	log.time_stamp("cycle");
	white.insert(gray.begin(), gray.end());
	gray.clear();

	// black and gray are empty, all objects in white
	std::for_each(root.begin(), root.end(), Juggle<S>::blackener(white, gray));

	while (not gray.empty())
	{
		T *a = *gray.begin();
		Juggle<S>::blacken(white, gray, a);
		Juggle<S>::move(gray, black, a);
	}

	for (auto a : white)
	{
		log.entry("expunging: ", a->spec());
		delete a;
	}

	white.clear();
	white.insert(black.begin(), black.end());
	black.clear();
}

template <typename T>
void Scarlett::GC<T>::cycle(bool force = false)
{
	T::_gc().do_cycle(force);
}

