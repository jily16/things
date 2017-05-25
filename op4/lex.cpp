#include "stdafx.h"

#include "lex.h"
#include <iostream>
using std::distance;

inline bool isnum(char _Ch) { return _Ch >= '0' && _Ch <= '9'; }

Lexer::Lexer(string const & _In)
	:error_place(-1), buf(_In), cur_it(buf.cbegin()), end_it(buf.cend())
{}

shared_ptr<Token> Lexer::get_next_token() throw(std::logic_error)
{
	if (cur_it != end_it)
	{
		if (*cur_it == '(')
		{
			++cur_it;
			return make_shared<lbracket_t>(distance(buf.cbegin(), cur_it));
		}
		if (*cur_it == ')')
		{
			++cur_it;
			return make_shared<rbracket_t>(distance(buf.cbegin(), cur_it));
		}
		if (*cur_it == '+')
		{
			++cur_it;
			return make_shared<plus_t>(distance(buf.cbegin(), cur_it));
		}
		if (*cur_it == '-')
		{
			++cur_it;
			return make_shared<minus_t>(distance(buf.cbegin(), cur_it));
		}
		if (*cur_it == '*')
		{
			++cur_it;
			return make_shared<multiply_t>(distance(buf.cbegin(), cur_it));
		}
		if (*cur_it == '/')
		{
			++cur_it;
			return make_shared<divide_t>(distance(buf.cbegin(), cur_it));
		}
		if (isnum(*cur_it))
		{
			shared_ptr<num_t> token = make_shared<num_t>(distance(buf.cbegin(), cur_it));
			while (cur_it != end_it && isnum(*cur_it))
			{
				token->value *= 10;
				token->value += (*cur_it - '0');
				++cur_it;
			}
			if (cur_it != end_it && *cur_it == '.')
			{
				++cur_it;
				if (cur_it == end_it || !isnum(*cur_it))
				{
					error_place = std::distance(buf.cbegin(), cur_it) + 1;
					throw std::logic_error("Wrong number: there must be a digit after a dot!");
				}
				int depth = -1;
				while (cur_it != end_it && isnum(*cur_it))
				{
					token->value += ((*cur_it) - '0') * pow(10, depth--);
					++cur_it;
				}
			}
			return token;
		}
	}
	else
	{
		return make_shared<Token>(buf.size());
	}
}
