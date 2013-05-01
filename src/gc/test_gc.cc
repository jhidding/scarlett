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

#include "gc.h"

#ifdef UNITTEST
#include "../unittest/test.h"
#include "../exception.h"
#include <initializer_list>

using namespace Scarlett;

namespace GCTest {
	class Object;

	typedef Object *ptr;
	typedef std::function<void (ptr)> Marker;

	class Object
	{
		friend class GC<Object>;
		static std::unique_ptr<GC<Object>> _gc_ptr;
		static GC<Object> &_gc() {
			if (not _gc_ptr) _gc_ptr = std::unique_ptr<GC<Object>>(new GC<Object>);
			return *_gc_ptr;
		}

		public:
			void add_to_root()
				{ _gc().add_to_root(this); }
			void remove_from_root()
				{ _gc().remove_from_root(this); }

			void *operator new(size_t size)
				{ return _gc().alloc_object(size); }
			virtual void gc(Marker const &mark) const
				{}

			virtual std::string type_name() const 
				{ return "anonymous"; }

			std::string spec() const
			{
				std::ostringstream ss;
				ss << "[" << this->type_name() << "|" << this << "]";
				return ss.str();
			}

			virtual std::string repr() const { return spec(); }

			virtual ~Object() {}
	};

	std::unique_ptr<GC<Object>> Object::_gc_ptr;

	template <typename T>
	class Static: public T
	{
		public:
			void *operator new(size_t size)
				{ throw Exception(ERROR_memory, "cannot allocate a Static<> object."); }

			Static()
			{
				T::add_to_root();
			}

			Static(T const &q):
				T(q)
			{
				T::add_to_root();
			}

			~Static()
			{
				T::remove_from_root();
			}
	};

	class Pair: public Object
	{
		public:
			ptr a, b;

			Pair(ptr a_, ptr b_): a(a_), b(b_) {}

			virtual void gc(Marker const &mark) const
				{ mark(a); mark(b); }

			virtual std::string type_name() const { return "pair"; }
			virtual std::string repr() const
			{
				std::stringstream s;
				s 	<< "[" << type_name() << " | " << this << " | "
					<< a << " . " << b << "]";
				return s.str();
			}
	};

	class Vector: public Object, public std::vector<ptr>
	{
		public:
			Vector() {}

			Vector(std::initializer_list<ptr> const &list):
				std::vector<ptr>(list) {}

			virtual void gc(Marker const &mark) const
				{ std::for_each(begin(), end(), mark); }

			virtual std::string type_name() const 
				{ return "vector"; }

			virtual std::string repr() const
			{
				std::stringstream s;
				s 	<< "[" << type_name() << " | " << this << " | "
					<< " size " << size() << "]";
				return s.str();
			}
	};

	class Mock: public Object
	{
		static size_t num;
		size_t id;

		public:
			static std::map<size_t, bool> is;

			static bool exists(size_t i) 
			{ 
				if (is.count(i) > 0) 
					return is[i];
				else
					return false;
			}

			Mock(): id(num++) { is[id] = true; }

			~Mock() { is[id] = false; }
	};

	size_t Mock::num = 0;
	std::map<size_t, bool> Mock::is;

	Test::Unit GC_unittest(
			"0001 - garbage collector",
			"", [] ()
	{
		Static<Vector> v({ new Mock, new Mock, new Mock });
		unsigned n = std::count_if(Mock::is.begin(), Mock::is.end(),
			[] (std::pair<size_t, bool> const &p) { return p.second; });
		if (n != 3) throw(Exception(ERROR_fail, "initial state incorrect."));
		GC<Object>::cycle(true);
		n = std::count_if(Mock::is.begin(), Mock::is.end(),
			[] (std::pair<size_t, bool> const &p) { return p.second; });
		if (n != 3) throw(Exception(ERROR_fail, "objects should not have been deleted."));
		v.clear();
		GC<Object>::cycle(true);
		n = std::count_if(Mock::is.begin(), Mock::is.end(),
			[] (std::pair<size_t, bool> const &p) { return p.second; });
		if (n != 0) throw(Exception(ERROR_fail, "objects should have been deleted now."));

		return true;
	});
}

#endif


// vim:sw=4:ts=4:tw=72
