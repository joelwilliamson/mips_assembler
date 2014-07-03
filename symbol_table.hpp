#pragma once

#include <unordered_map>
#include "parser.hpp"
#include <stdint>
#include <string>

typedef uint32_t offset_t;
typedef std::string label_t;
typedef std::unordered_map<label_t,offset_t> symbol_table_t;
