#ifndef ASMINSTRUCTION
#define ASMINSTRUCTION

#include "assembler/parser.hpp"
#include <cstdint>

struct ASMInstruction {
	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint16_t imm;
	uint32_t addr;
 
	ASMInstruction encode(const ASTNode node);
	void print();

  uint32_t encodeRType(const ASTNode& node);
  uint32_t encodeIType(const ASTNode& node);
  uint32_t encodeJType(const ASTNode& node);
  uint32_t encodePseudo(const ASTNode& node);

};

#endif // !ASMINSTRUCTION
