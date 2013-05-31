
namespace Scarlett
{
	class Implements: public C_class
	{
		Type	*m_type;

		public:
			Type
	};

	/*! type information
	 *
	 * A spec is a specification of the interface that an object
	 * should subject to. It says nothing about the implementation
	 * that is provided. If a function works on variables of a
	 * certain type, iit should work on all variables of that type
	 * in spite of different underlying representations.
	 *
	 * Say we have a spec T that specifies a composition operator *,
	 * We'd like to be able to specify (* T) as a member of T,
	 * with the restriction that the argument must have the same
	 * type. Then we can implement functions that work universally
	 * on all types that implement T.
	 *
	 * On the other hand, when working with sequence types,
	 * we'd like to be able to mix different types in our operations.
	 * As long as they implement some method of retrieving a ptr
	 * from the head.
	 *
	 * This class stores all information on types in Scarlett.
	 * Identification of the type of an object should follow
	 * by the address of an instance of this class. This means
	 * that each type has only one instance.
	 */
	class Type: public Object
	{
		Global<Type>	m_type;
		std::string 	m_name;
		ptr		m_methods;
		Set		*m_parents;

		public:
			void gc(Edict const &cmd) const 
				{ cmd(m_methods); cmd(m_parents); }

			Type *type() const { return &m_type; }

			/*! returns the name of the type.
			 *
			 * The type name should not be used as an id,
			 * rather a friendly way of saying hello.
			 */
			std::string name() const { return m_name; }

			/*! returns a list of methods
			 *
			 * A method is a pair, the car being a symbol,
			 * the cdr a pattern that should match the argument
			 * tree of the method.
			 */
			ptr provides() const { return m_methods; }

			/*! only if type derives from argument > return true.
			 *
			 * In the hierarchy of types, one type can be said to
			 * be 'bigger' than another if the other is contained
			 * in the list of parents.
			 */
			bool operator>(ptr other) const
			{
				return m_parents->count(other) > 0;
			}
	};
}

