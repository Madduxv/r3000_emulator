#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include "assembler/parser.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, uint16_t> r_type;
extern std::unordered_map<std::string, uint16_t> j_type;
extern std::unordered_map<std::string, uint16_t> i_type;
extern std::unordered_map<std::string, uint16_t> registers;
extern std::unordered_map<std::string, uint32_t> labels;
extern std::unordered_map<std::string, uint16_t> pseudo;

void getSymbols(const std::vector<ASTNode>& ast, Memory& mem);
void resolveSymbols(std::vector<ASTNode>& ast);
void allocateAscii(const std::vector<ASTNode>& ast, Memory& mem, int& addrPtr, const int i, int& j);

#endif // !SYMBOLS_HPP
