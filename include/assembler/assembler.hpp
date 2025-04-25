#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "assembler/parser.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <string>
#include <vector>


struct Assembler {
  std::vector<ASTNode> AST;
  std::vector<uint32_t> Instructions;
  uint32_t varAddrPtr;
  uint32_t instrAddrPtr;

  uint32_t setStart(Memory& mem);

public:
  Assembler(const std::string& fileName, Memory& mem);
  void assemble(Memory& mem);
  void print();
};

std::string readFile(const std::string& filename);

#endif // !ASSEMBLER_HPP
