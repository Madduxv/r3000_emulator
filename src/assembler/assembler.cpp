#include "assembler/assembler.hpp"
#include <cstdint>
#include <vector>
#include <string>

uint32_t Assembler::encodeRType(const ASTNode& node) {
  return 0;
}

uint32_t Assembler::encodeIType(const ASTNode& node) {
  return 0;
}

uint32_t Assembler::encodeJType(const ASTNode& node) {
  return 0;
}

void Assembler::insertGlobal() {

}

Assembler::Assembler(const std::vector<ASTNode>& AST): asciiAddrPtr(0), intAddrPtr(0) {
  this->AST = AST;
}

std::vector<uint8_t> Assembler::assemble(const std::string& filename) {
  return {};
}
