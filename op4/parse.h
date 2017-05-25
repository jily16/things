#pragma once
#include "lex.h"
#include <iostream>

/****************
E -> T + E | T - E | T
T -> F * T | F / T | F
F -> (E) | num | -num
****************/

struct ASTNode
{
	shared_ptr<Token> token_name;
	shared_ptr<ASTNode> lchild, rchild;
};

class Parser
{
public:
	Parser(string const &_In);

	//do calculation
	double go();

	bool err_set() const;

	void log_err(std::exception const &_E);

private:
	Lexer lexer;

	int error_place;

	shared_ptr<Token> token;

	shared_ptr<ASTNode> ast;

	void parse();

	/*After each function below, current token will be of the new symbol.
	So there is no need to manually get_next_token() at the beginning of functions.*/

	shared_ptr<ASTNode> E();

	shared_ptr<ASTNode> T();

	shared_ptr<ASTNode> F();

	double calc_node(shared_ptr<ASTNode> const &_Node);
};