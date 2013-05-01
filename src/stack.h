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
#include <stack>
#include <algorithm>
#include "object.h"
#include "pair.h"

namespace Scarlett
{
	class Stack: public Object
	{
		size_t  m_size;
		ptr 	m_top;

		public:
			Stack() { m_top = &nil; }

			ptr top() const 
			{ 
				if (is_pair(m_top))
					return car(m_top);
				else
					throw Exception(ERROR, "empty stack is topless.");
			}

			void pop() 
			{
				if (is_pair(m_top))
				{
					m_top = cdr(m_top); --m_size;
				}
				else throw Exception(ERROR, "cannot pop an empty stack.");
			}

			bool empty() const 
			{ 
				return is_nil(m_top); 
			}

			void push(ptr q) 
			{
				m_top = cons(q, m_top); ++m_size; 
			}

			size_t size() const { return m_size; }

			virtual void gc(Edict const &cmd) const
				{ cmd(m_top); }

			std::string type_name() const 
				{ return "stack"; }

			virtual std::string repr() const
			{
				std::stringstream s;
				s 	<< "[" << type_name() << " | " << this << " | "
					<< " size " << size() << "]";
				return s.str();
			}
	};
}

// vim:sw=4:ts=4:tw=72
