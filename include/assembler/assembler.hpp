#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "assembler/parser.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct Assembler {
  std::vector<uint8_t> machineCode;
  std::vector<ASTNode> AST;
  uint32_t asciiAddrPtr;
  uint32_t intAddrPtr;

  uint32_t encodeRType(const ASTNode& node);
  uint32_t encodeIType(const ASTNode& node);
  uint32_t encodeJType(const ASTNode& node);
  void insertGlobal();

public:
  Assembler(const std::vector<ASTNode>& AST);
  std::vector<uint8_t> assemble(const std::string& filename);
};



#endif // !ASSEMBLER_HPP
