#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include "assembler/parser.hpp"
#include <cstdint>
#include <map>
#include <string>

extern std::map<std::string, uint16_t> r_type;
extern std::map<std::string, uint16_t> j_type;
extern std::map<std::string, uint16_t> i_type;
extern std::map<std::string, uint16_t> registers;
extern std::map<std::string, uint32_t> labels;

void getSymbols(const std::vector<ASTNode>& ast);
void resolveSymbols(std::vector<ASTNode>& ast);

#endif // !SYMBOLS_HPP
