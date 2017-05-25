#pragma once
#include <exception>
#include <memory>
#include <string>

using std::string;
using std::shared_ptr;
using std::make_shared;

struct Token
{
	size_t index;
	Token(size_t _Index) :index(_Index) {}
	virtual ~Token() {}
};
struct num_t :public Token
{
	double value;
	num_t(size_t _Index) :Token(_Index), value(0.0) {}
};
struct plus_t :public Token
{
	plus_t(size_t _Index) :Token(_Index) {}
};
struct minus_t :public Token
{
	minus_t(size_t _Index) :Token(_Index) {}
};
struct multiply_t :public Token
{
	multiply_t(size_t _Index) :Token(_Index) {}
};
struct divide_t :public Token
{
	divide_t(size_t _Index) :Token(_Index) {}
};
struct lbracket_t :public Token
{
	lbracket_t(size_t _Index) :Token(_Index) {}
};
struct rbracket_t :public Token
{
	rbracket_t(size_t _Index) :Token(_Index) {}
};

class Lexer
{
public:
	Lexer(string const &_In);

	//no blank allowed
	shared_ptr<Token> get_next_token() throw(std::logic_error);

	int error_place;
private:
	string const buf;

	string::const_iterator cur_it;

	string::const_iterator end_it;
};