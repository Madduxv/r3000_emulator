#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "assembler/parser.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <string>
#include <vector>

struct Assembler {
  /*Memory mem;*/
  std::vector<ASTNode> AST;
  uint32_t varAddrPtr;
  uint32_t instrAddrPtr;

  uint32_t encodeRType(const ASTNode& node);
  uint32_t encodeIType(const ASTNode& node);
  uint32_t encodeJType(const ASTNode& node);
  uint32_t encodePseudo(const ASTNode& node);
  uint32_t setStart(Memory& mem);

  void insertVariable(const ASTNode& node);

public:
  Assembler(const std::string& fileName, Memory& mem);
  std::vector<uint8_t> assemble();
  void insertInstr(uint32_t addr, uint32_t instr);
};

std::string readFile(const std::string& filename);


#endif // !ASSEMBLER_HPP
