#include "assembler/asminstruction.hpp"
#include "assembler/symbols.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>

uint32_t ASMInstruction::encode(const ASTNode node) {
  if (r_type.find(node.val) != r_type.end()) {
    encodeRType(node);
  } else if (i_type.find(node.val) != i_type.end()) {
    encodeIType(node);
  } else if (j_type.find(node.val) != j_type.end()) {
    encodeJType(node);
  } else if (pseudo.find(node.val) != pseudo.end()) {
    encodePseudo(node);
  } else {
    std::cout << "ERROR: Unknown Instruction (" << node.val << ")" << std::endl;
    exit(1);
  }
  return 0;
}
uint32_t ASMInstruction::encodeRType(const ASTNode& node) {
  opcode = 0;
  funct = r_type[node.val];
  return (opcode << 26) + funct;
}
uint32_t ASMInstruction::encodeIType(const ASTNode& node) {
  opcode = r_type[node.val];
  return opcode << 26;
}
uint32_t ASMInstruction::encodeJType(const ASTNode& node) {
  opcode = r_type[node.val];
  return opcode << 26;
}
uint32_t ASMInstruction::encodePseudo(const ASTNode& node) {
  return 0;
}
