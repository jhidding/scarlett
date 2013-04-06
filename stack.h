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
