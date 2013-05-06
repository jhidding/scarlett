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

#include "gc/gc.h"
#include "exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <typeinfo>

namespace Scarlett
{
	class Object;
	typedef Object *ptr;
	typedef std::function<void (ptr)> Edict;

	template <typename T>
	inline T *cast_ptr(ptr obj) 
	{ return reinterpret_cast<T *>(obj); }

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
			virtual void gc(Edict const &cmd) const
				{}

			virtual bool is_mutable() const 
				{ return false; }
			virtual std::string type_name() const 
				{ return "anonymous"; }
			virtual bool is_equiv(ptr other) const
				{ return other == this; }

			std::string spec() const {
				std::ostringstream ss;
				ss << "[" << type_name() << "|" << this << "]";
				return ss.str();
			}

			virtual std::string repr() const
			{
				std::ostringstream ss;
				ss << "[" << type_name() << "|" << this << "]";
				return ss.str();
			}

			virtual std::string str() const
			{
				return this->repr();
			}

			virtual ~Object() {}
	};

	extern bool is_equiv(ptr a, ptr b);
	extern bool is_equal(ptr a, ptr b);
	extern std::string repr(ptr a);

	template <typename T>
	class Static: public T
	{
		public:
			void *operator new(size_t size)
				{ throw Exception(ERROR_memory, "cannot allocate a Static<> object."); }

			void operator delete(void *)
				{ throw Exception(ERROR_memory, "cannot delete a Static<> object."); }

			Static()
			{
				T::add_to_root();
			}

			template <typename ...Args>
			Static(Args &&...args):
				T(std::forward<Args>(args)...)
			{
				T::add_to_root();
			}

			Static(T const &q):
				T(q)
			{
				T::add_to_root();
			}

			virtual ~Static()
			{
				T::remove_from_root();
			}
	};

	class Symbol: public Object
	{
		std::string m_name;

		public:
			Symbol(std::string const &name): m_name(name) {}

			std::string type_name() const { return "symbol"; }
			std::string repr() const { return m_name; }
			std::string const &value() const { return m_name; }

			bool is_equiv(ptr other) const
			{ return m_name == cast_ptr<Symbol>(other)->m_name; }
	};

	inline bool is_symbol(ptr q) { return q->type_name() == "symbol"; }
	inline Symbol *operator "" _s(char const *c, size_t s) { return new Symbol(c); }

	class Special: public Object
	{
		std::string const m_name;

		public:
			Special(std::string const &name): m_name(name) {}

			std::string type_name() const { return "special"; }
			std::string repr() const { return "#" + m_name; }
	};

	extern Static<Special> inert;
	extern Static<Special> ignore;
	extern Static<Special> nil;
	extern Static<Special> infinity;

	inline bool is_nil(ptr q) { return q == &nil; }
	inline bool is_ignore(ptr q) { return q == &ignore; }
	inline bool is_inert(ptr q) { return q == &inert; }

	template <typename T>
	class Atom_base: public Object
	{
		T m_value;

		public:
			Atom_base(T _value): m_value(_value) {}

			std::string type_name() const { return "atomic"; }
	
			std::string repr() const
			{
				std::ostringstream ss;
				ss << m_value;
				return ss.str();
			}

			T const &value() const { return m_value; }
			T &ref() { return m_value; }
			operator T() const { return m_value; }

		/*	bool is_equiv(ptr other) const
			{
				return cast_ptr<Atom_base<T>>(other)->m_value == m_value;
			} */
	};

	template <typename T>
	class Atom: public Atom_base<T>
	{
		public:
			Atom(T _value): Atom_base<T>(_value) {}
	};

	template <>
	class Atom<bool>: public Atom_base<bool>
	{
		public:
			Atom<bool>(bool a): Atom_base<bool>(a) {}
			std::string type_name() const { return "boolean"; }
			std::string repr() const { return (value() ? "#t" : "#f"); }
	};

	typedef Atom<bool> Boolean;
	inline bool is_boolean(ptr a) { return a->type_name() == "boolean"; }

	inline ptr operator"" _a(unsigned long long a) { return new Atom<unsigned>(a); }
	inline ptr operator"" _a(long double a) { return new Atom<double>(a); }
}

// vim:sw=4:ts=4:tw=72
