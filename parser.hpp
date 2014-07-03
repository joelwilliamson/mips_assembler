#pragma once
#include <istream>
#include <deque>
#include "utility.hpp"

enum class token_type {
	INST, // Instruction
	LABEL_DECL, // Label declaration
	LABEL, // Label use
	REG, // Register
	PINT, // Positive integer constant
	NINT, // Negative integer constant
	HEX, // Hexadecimal constant
	INVALID, // Not an actual token
	};

class token {
	public:
	token(std::string symbol, token_type type);
	token();
	std::string symbol;
	token_type type;
	};

utility::optional<std::deque<token>> parse(std::istream& is);
