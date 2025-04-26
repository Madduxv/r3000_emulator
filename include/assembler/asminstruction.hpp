#ifndef ASMINSTRUCTION
#define ASMINSTRUCTION

#include "assembler/parser.hpp"
#include <cstdint>

struct ASMInstruction {
 
	uint32_t encode(const ASTNode node, uint32_t currentPc);

  private:
  uint32_t encodeRType(const ASTNode& node);
  uint32_t encodeIType(const ASTNode& node, uint32_t currentPc);
  uint32_t encodeJType(const ASTNode& node);
  uint32_t encodePseudo(const ASTNode& node);

  bool isRType(const ASTNode& node);
  bool isIType(const ASTNode& node);
  bool isJType(const ASTNode& node);
  bool isPseudo(const ASTNode& node);

};

#endif // !ASMINSTRUCTION
