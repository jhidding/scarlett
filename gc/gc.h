#pragma once
#include <functional>
#include <algorithm>
#include <set>

#include "../log/log.h"

namespace Scarlett
{
	template <typename T>
	class GC
	{
		typedef std::set<T *> S;
		S root, white, gray, black;

		public:
			typedef std::function<void (T *)> Edict;

		private:
			static void move(S &A, S &B, T *q)
			{
				if (A.count(q) == 0) return;

				A.erase(q);
				B.insert(q);
			}

			static Edict mover(S &A, S &B)
			{
				return [&A, &B] (T *q)
				{
					move(A, B, q);
				};
			}

			static void blacken(S &A, S &B, T *q)
			{
				if (q == NULL) return;
				q->gc(mover(A, B));
			}

			static Edict blackener(S &A, S &B)
			{
				return [&A, &B] (T *q)
				{
					blacken(A, B, q);
				};
			}

			static void destroyer(T *q)
			{
				delete q;
			}

		public:
			Log log;

			GC():
				log("gc")
			{
				//T::deity = this;
			}

			void *alloc_object(size_t size)
			{
				void *mem = ::operator new(size);
				log.entry("new object: ", mem);
				white.insert(reinterpret_cast<T *>(mem));
				return mem;
			}

			void add_to_root(T *q)
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

			void remove_from_root(T *q)
			{
				root.erase(q);
			}

			static void cycle()
			{
				T::_gc().do_cycle();
			}

			void do_cycle()
			{
				log.time_stamp("cycle");

				// black and gray are empty, all objects in white
				std::for_each(root.begin(), root.end(), blackener(white, gray));

				while (not gray.empty())
				{
					T *a = *gray.begin();
					blacken(white, gray, a);
					move(gray, black, a);
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
	};
}

// vim:sw=4:ts=4:tw=72
