#include "parser.hpp"

token::token(std::string symbol, token_type type)
	: symbol(symbol), type(type)
	{}
