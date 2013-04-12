#include "list.h"
#include "hash.h"
#include "sign.h"
#include "ellipsis.h"

using namespace Scarlett;
using namespace Read;

Continuation *Scarlett::Read::Dot::put(int ch)
{
	if (isspace(ch))
	{
		cast_ptr<Reader>(parent())->poke(Improper);
		return new CdrLiteral(parent());
	}

	if (isdigit(ch))
	{
		return cast_ptr<Reader>((new Number(parent()))->put('.'))->put(ch);
	}

	if (ch == '.')
	{
		cast_ptr<Reader>(parent())->poke(Circular);
		return new Ellipsis(parent());
	}

	throw Exception(ERROR_syntax, 
			"'.' should be followed by digit for a number, "
			"or by white space to make a pair.");
}

Continuation *Scarlett::Read::ListLiteral::put(int ch)
{
	if (ch == '#') return new Hash(this);
	if (ch == '"') return new StringLiteral(this);
	if (ch == '(') return new ListLiteral(this);
	if (ch == ';') return new Comment(this);
	if (ch == '.') return new Dot(this);
	if (ch == '+' or ch == '-') return new Sign(this, ch);

	if (isdigit(ch))
		return (new Number(this))->put(ch);

	if (isspace(ch)) return this;

	if (ch == ')') 
	{
		if (improper)
			return parent()->supply(append_reverse(cdr(rev_lst), car(rev_lst)));

		if (circular)
		{
			ptr p = reverse(rev_lst);
			encycle(p, proper_list_length(p) - 1, 1);
			return parent()->supply(p);
		}

		return parent()->supply(reverse(rev_lst));
	}

	if (ch == EOF)
		throw Exception(ERROR_syntax, "premature EOF");

	return (new SymbolLiteral(this))->put(ch);
}

Continuation *Scarlett::Read::CdrLiteral::put(int ch)
{
	if (ch == ')') throw Exception(ERROR_syntax, 
		"didn't expect ')' immediately after '.' ");
	if (ch == '.') throw Exception(ERROR_syntax, 
		"found erronous ' . . '");

	if (ch == '#') return new Hash(parent());
	if (ch == '"') return new StringLiteral(parent());
	if (ch == '(') return new ListLiteral(parent());
	if (ch == ';') return new Comment(this);
	if (ch == '+' or ch == '-') return new Sign(this, ch);

	if (isdigit(ch) or ch == '.')
		return (new Number(parent()))->put(ch);

	if (isspace(ch)) return this;

	return (new SymbolLiteral(parent()))->put(ch);
}

