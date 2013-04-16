#include "pair.h"

using namespace Scarlett;

Scarlett::List_metric Scarlett::c_get_list_metric(ptr l)
{
	ptr k = l;
	int p = 0, c = 0;

	while (is_pair(l) and mark(l) < 2)
	{
		switch (mark(l))
		{
			case 0: ++p;
				break;

			case 1: ++c;
				break;
		}

		set_mark(l, mark(l) + 1);
		l = cdr(l);
	}

	int n = (is_nil(l) ? 1 : 0), a = p - c;

	deep_mark(k, 0);
	return { p, n, a, c };
}

ptr Scarlett::get_list_metric(ptr l)
{
	ptr k = l;
	int p = 0, c = 0;

	while (is_pair(l) and mark(l) < 2)
	{
		switch (mark(l))
		{
			case 0: ++p;
				break;

			case 1: ++c;
				break;
		}

		set_mark(l, mark(l) + 1);
		l = cdr(l);
	}

	int n = (is_nil(l) ? 1 : 0), a = p - c;

	deep_mark(k, 0);
	return list(
		new Atom<int>(p), new Atom<int>(n),
		new Atom<int>(a), new Atom<int>(c));
}

