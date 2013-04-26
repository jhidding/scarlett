#include "environment.h"
#include "system.h"
#include "safety.h"
#include "system.h"
#include "../iface.h"

using namespace Scarlett;

Continuation *make_environment(Continuation *C, Environment *env, ptr args)
{
	assert_that("make-environment", args, Is_proper_list());
	return C->supply(new Environment(args));
}

Global<C_applicative> Scarlett::Make_environment("make-environment", make_environment);
Global<C_applicative> Is_environment("environment?", Curry<bool, ptr>(is_environment));

// Environment impl. --------------------------------------------

void env_bind(ptr name_, ptr value, std::map<std::string, ptr> &B)
{
	std::string name = cast_ptr<Symbol>(name_)->value();
	if (B.count(name) > 0)
		throw Exception(ERROR_bound, name);

	B[name] = value;
}

std::function<void (ptr, ptr)> env_binder(std::map<std::string, ptr> &B)
{
	return [&B] (ptr name_, ptr value)
	{
		env_bind(name_, value, B);
	};
}

Environment::Environment(ptr env_lst):
	parent_list(env_lst), bindings()
{}

Environment::Environment(ptr args, ptr pars, ptr env_lst):
	parent_list(env_lst), bindings()
{
	match_tree(args, pars, env_binder(bindings));
}

void Environment::define(ptr args, ptr pars)
{
	match_tree(args, pars, env_binder(bindings));
}

ptr Environment::look_up(std::string const &name)
{
	if (bindings.count(name) == 0)
	{
		if (is_nil(parent_list))
			throw Exception(ERROR_unbound, name);

		for (ptr e : List(parent_list))
		{
			try 
			{
				return cast_ptr<Environment>(e)->look_up(name);
			}
			catch (Exception const &e)
			{}
		}

		throw Exception(ERROR_unbound, name);
	}
	else
	{
		return bindings[name];
	}
}

ptr Environment::look_up(ptr q)
{
	return look_up(cast_ptr<Symbol>(q)->value());
}

void Environment::print_map(std::ostream &out)
{
	for (auto kv : bindings)
	{
		out << std::setw(20) << kv.first
			<< " -> " << kv.second->repr() << std::endl;
	}
}
