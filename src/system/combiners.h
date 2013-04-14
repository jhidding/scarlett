#pragma once
#include "../object.h"
#include "continuation.h"
#include "../misc/format.h"

#include <functional>

namespace Scarlett
{
	typedef std::function<Continuation *(Continuation *, Environment *, ptr)> CEO;

	class Applicative;

	class Combiner: public Object
	{
		public:
			virtual Applicative *wrap() = 0;

			virtual Continuation *operator()(Continuation *C,
				Environment *env, ptr args) = 0;
	};

	class Operative: public Combiner
	{
		public:
			virtual std::string type_name() const
				{ return "operative"; }

			virtual Applicative *wrap();
	};

	class Applicative: public Combiner
	{
		public:
			virtual std::string type_name() const
				{ return "applicative"; }

			virtual Applicative *wrap()
				{ return this; }

			virtual Operative *unwrap();
	};

	inline bool is_operative(ptr a) { return a->type_name() == "operative"; }
	inline bool is_applicative(ptr a) { return a->type_name() == "applicative"; }
	inline bool is_combiner(ptr a) { return is_applicative(a) or is_operative(a); }

	class Wrapped_operative: public Applicative
	{
		Operative *func;

		public:
			void gc(Edict const &cmd) const
				{ cmd(func); }

			Wrapped_operative(Operative *func_):
				func(func_) {}

			Continuation *operator()(Continuation *C, Environment *env, ptr args)
				{ return (*func)(C, env, args); }

			Operative *unwrap()
				{ return func; }
	};

	class Unwrapped_applicative: public Operative
	{
		Applicative *func;
		
		public:
			void gc(Edict const &cmd) const 
				{ cmd(func); }

			Unwrapped_applicative(Applicative *func_):
				func(func_) {}

			Continuation *operator()(Continuation *C, Environment *env, ptr args)
				{ return (*func)(C, env, args); }

			Applicative *wrap()
				{ return func; }
	};

	inline Applicative *Operative::wrap() { return new Wrapped_operative(this); }
	inline Operative *Applicative::unwrap() { return new Unwrapped_applicative(this); }

	class Apply: public Continuation
	{
		Environment		*env;
		Combiner		*func;
		ptr				args;

		public:
			virtual std::string state() const
			{ return Misc::format("[ Apply | ", func->repr(), " ", args->repr(), "]"); }

			void gc(Edict const &cmd) const
			{
				Continuation::gc(cmd);
				cmd(env); cmd(func); cmd(args);
			}

			Apply(Continuation *parent_, Environment *env_, Combiner *func_):
				Continuation(parent_), env(env_),
				func(func_), args(&nil)
			{}

			virtual Continuation *supply(ptr args_)
				{ args = args_; return this;}

			virtual Continuation *apply()
				{ return (*func)(parent(), env, args); }
	};

	class C_applicative: public Applicative
	{
		CEO fn;

		public:
			C_applicative(CEO const &fn_): 
				fn(fn_) {}

			inline Continuation *operator()(
					Continuation *C, Environment *env, ptr args)
			{ return fn(C, env, args); }

			Operative *unwrap() 
				{ return new Unwrapped_applicative(this); }
	};

	class C_operative: public Operative
	{
		CEO fn;

		public: 
			C_operative(CEO const &fn_): 
				fn(fn_) {}

			inline Continuation *operator()(
					Continuation *C, Environment *env, ptr args)
				{ return fn(C, env, args); }
	};

	class C_closure: public Operative
	{
		CEO fn;
		ptr ref_list;

		public:
			C_closure(CEO const &fn_, ptr ref_list_ = &nil): 
				fn(fn_), ref_list(ref_list_) {}

			void gc(Edict const &cmd) const
				{ cmd(ref_list); }

			inline Continuation *operator()(
					Continuation *C, Environment *env, ptr args)
				{ return fn(C, env, args); }
	};
}

// vim:sw=4:ts=4:tw=72
