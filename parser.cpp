#include "parser.hpp"

#include <unordered_set>
#include <sstream>
#include <istream>
#include <algorithm>

#define REGISTER_NUMBER 32

using utility::optional;
using utility::log;
using utility::log_level;

token::token(std::string symbol, token_type type)
	: symbol(symbol), type(type)
	{}

token::token()
	: symbol(),type(token_type::INVALID)
	{}

optional<std::string> extract_word(std::string& in)
	{
	std::string::size_type end_pos{in.find_first_of(" \t")};
	if (end_pos == std::string::npos) { // Word break not found
		return optional<std::string>::nothing();
		}
	
	auto result = optional<std::string>::just(in.substr(0,end_pos));
	in.erase(0,end_pos);
	return result;
	}

// All instructions must be put in this set in canonical (lower-case) form
static const std::unordered_set<std::string> instructions{
	"add","sub","mult","multu","div","divu",
	"mfhi","mflo","lis","lw","sw","slt","sltu",
	"beq","bne","jr","jalr",".word"};

optional<std::deque<token>> parse(std::istream& is)
	{
	std::string line;
	bool error_detected{false};
	std::deque<token> result;
	while (std::getline(is,line)) {
		std::stringstream ss{line};
		std::string next_token;
		while (ss >> next_token) {
			if (next_token.front() == ';') { // This is a comment
				log(std::string{"Found comment starting with "} +next_token);
				break;
				}
			else if (instructions.find(next_token)!=instructions.cend()) {
				log(std::string{"Found instruction: "}+next_token);
				result.emplace_back(next_token,token_type::INST);
				}
			else if (next_token.front() == '$') {
				log(std::string{"Found register: "}+next_token);
				try {
					unsigned long reg_number = std::stoul(next_token);
					if (reg_number >= REGISTER_NUMBER) {
						log("Invalid register number",log_level::ERR);
						error_detected = true;
						}
					}
				catch (...) {
					log("Invalid register number",log_level::ERR);
					error_detected = true;
					}
				result.emplace_back(next_token,token_type::REG);
				}
			else if (next_token.front() == '-') {
				log(std::string{"Found negative: "}+next_token);
				result.emplace_back(next_token,token_type::NINT);
				}
			else if (next_token.front() == '0' && next_token.length() > 2 &&
					next_token.at(1) == 'x') {
				log(std::string{"Found hex const: "}+next_token);
				if (!std::all_of(next_token.begin()+2, // Ignore 0x
						next_token.end(),
						[](char c){return std::isxdigit(c);})){
					log(std::string{"Invalid number: "}+next_token,log_level::ERR);
					error_detected = true;
					}
				result.emplace_back(next_token,token_type::HEX);
				}
			else if (std::isdigit(next_token.front())) {
				log(std::string{"Found positive constant: "}+next_token);
				if (!std::all_of(next_token.begin(),next_token.end(),
						[](char c){return std::isdigit(c);})) {
					log(std::string{"Invalid number: "}+next_token,log_level::ERR);
					error_detected = true;
					}
				result.emplace_back(next_token,token_type::PINT);
				}
			else if (next_token.back() == ':') {
				log(std::string{"Found label declaration: "}+ next_token);
				if (instructions.find(next_token.substr(0,next_token.length()-1))
						!= instructions.cend()) {
					// This label is an instruction identifier
					log(std::string{"Trying to use instruction name for label: "}
						+ next_token, log_level::WARNING);
					error_detected = true;
					}
				result.emplace_back(next_token,token_type::LABEL);
				}
			else { // Label
				log(std::string{"Found label use: "}+next_token);
				result.emplace_back(next_token,token_type::LABEL);
				}
			}
		}
	if (error_detected) {
		return optional<std::deque<token>>::nothing();
		}
	return optional<std::deque<token>>::just(std::move(result));
	}


#ifdef TESTING
int main() {
	set_log_level(log_level::DEBUG);
		{
		std::string program1{"ALABEL: .word 0x34a2 ;first line\n"
				".word -34 ; second line\n"
				".word 0xa43D9\n"
				".word 0xadg\n"
				".word 567a\n"};
		std::istringstream program_stream{program1};
		optional<std::deque<token>> maybe_tokens{parse(program_stream)};
		if (!maybe_tokens.present) {
			std::cout << "Program 1 encountered a serious error!" << std::endl;
			return 1;
			}
		std::cout << "Program 1 tokens:" << std::endl;
		for (const token& t : maybe_tokens.get_value()) {
			std::cout << t.symbol << std::endl;
			}
		}
	}
#endif
