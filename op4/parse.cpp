#include "stdafx.h"

#include "parse.h"

using std::dynamic_pointer_cast;
Parser::Parser(string const & _In)
	:lexer(Lexer(_In)), error_place(lexer.error_place)
{
	try
	{
		parse();
	}
	catch (std::logic_error const &_E)
	{
		log_err(_E);
	}
}

double Parser::go()
{
	return calc_node(ast);
}

bool Parser::err_set() const
{
	return error_place != -1;
}

void Parser::parse()
{
	token = lexer.get_next_token();
	ast = E();
}

shared_ptr<ASTNode> Parser::E()
{
	shared_ptr<ASTNode> Tresult = T(), result = make_shared<ASTNode>();
	if (dynamic_pointer_cast<plus_t>(token) || dynamic_pointer_cast<minus_t>(token))
	{
		if (dynamic_pointer_cast<plus_t>(token))
		{
			result->token_name = make_shared<plus_t>(-1);
		}
		else
		{
			result->token_name = make_shared<minus_t>(-1);
		}
		result->lchild = Tresult;
		token = lexer.get_next_token();
		result->rchild = E();
	}
	else
	{
		result = Tresult;
	}
	return result;
}

shared_ptr<ASTNode> Parser::T()
{
	shared_ptr<ASTNode> Fresult = F(), result = make_shared<ASTNode>();
	if (dynamic_pointer_cast<multiply_t>(token) || dynamic_pointer_cast<divide_t>(token))
	{
		if (dynamic_pointer_cast<multiply_t>(token))
		{
			result->token_name = make_shared<multiply_t>(-1);
		}
		else
		{
			result->token_name = make_shared<divide_t>(-1);
		}
		result->lchild = Fresult;
		token = lexer.get_next_token();
		result->rchild = T();
	}
	else
	{
		result = Fresult;
	}
	return result;
}

shared_ptr<ASTNode> Parser::F()
{
	shared_ptr<ASTNode> result = make_shared<ASTNode>();
	if (dynamic_pointer_cast<lbracket_t>(token))
	{
		token = lexer.get_next_token();
		shared_ptr<ASTNode> Eresult = E();
		if (!dynamic_pointer_cast<rbracket_t>(token))
		{
			error_place = token->index;
			throw std::logic_error("Unpaired bracket!");
		}
		result = Eresult;
	}
	else if (dynamic_pointer_cast<num_t>(token))
	{
		result->token_name = token;
	}
	else
	{
		error_place = token->index;
		throw std::logic_error("Incomplete expression!");
	}
	token = lexer.get_next_token();
	return result;
}

double Parser::calc_node(shared_ptr<ASTNode> const & _Node)
{
	shared_ptr<ASTNode> lc = _Node->lchild, rc = _Node->rchild;
	if (dynamic_pointer_cast<num_t>(_Node->token_name))
	{
		return dynamic_pointer_cast<num_t>(_Node->token_name)->value;
	}
	if (dynamic_pointer_cast<plus_t>(_Node->token_name))
	{
		return calc_node(_Node->lchild) + calc_node(_Node->rchild);
	}
	else if (dynamic_pointer_cast<minus_t>(_Node->token_name))
	{
		return calc_node(_Node->lchild) - calc_node(_Node->rchild);
	}
	else if (dynamic_pointer_cast<multiply_t>(_Node->token_name))
	{
		return calc_node(_Node->lchild)*calc_node(_Node->rchild);
	}
	else if (dynamic_pointer_cast<divide_t>(_Node->token_name))
	{
		double rresult = calc_node(_Node->rchild);
		if (rresult == 0)
		{
			throw std::runtime_error("Divided by 0!");
		}
		else
		{
			return calc_node(_Node->lchild) / rresult;
		}
	}
}

void Parser::log_err(std::exception const &_E)
{
	for (size_t i = 0; i < error_place; ++i)
	{
		std::cerr << ' ';
	}
	std::cerr << '^';
	std::cerr << _E.what() << std::endl << std::endl;
}
