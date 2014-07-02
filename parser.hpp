#pragma once
#include <istream>
#include <deque>

enum class token_type {
	INST, // Instruction
	CONST_US, // Short unsigned constant
	CONST_UL, // Long unsigned constant
	};

class token {
	public:
	token(std::string symbol, token_type type);
	std::string symbol;
	token_type type;
	};
