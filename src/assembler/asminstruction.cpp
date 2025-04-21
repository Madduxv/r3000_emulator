#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/symbols.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>

/**
 * @brief 
 * @param 
 * @return 
 */
uint32_t encodeRType(const ASTNode& node);
uint32_t ASMInstruction::encode(const ASTNode node, uint32_t address) {
  if (isRType(node)) {
    return encodeRType(node);
  } else if (isIType(node)) {
    return encodeIType(node);
  } else if (isJType(node)) {
    return encodeJType(node);
  } else if (isPseudo(node)) {
    return encodePseudo(node);
  } else {
    std::cout << "ERROR: Unknown Instruction (" << node.val << ")" << std::endl;
    exit(1);
  }
}

/**
 * @brief Encodes an R-type instruction into a 32-bit machine code format.
 *
 * @param node The AST node representing the instruction.
 * @return The encoded 32-bit machine instruction.
 */
uint32_t encodeRType(const ASTNode& node);
uint32_t ASMInstruction::encodeRType(const ASTNode& node) {
  // R-type
  /*| OPCODE |  RS   |  RT   |  RD   | SHAMT | FUNCT | */
  /*| 6-bits | 5-bit | 5-bit | 5-bit | 5-bit | 6-bit | */

  opcode = 0x00;
  funct = r_type.at(node.val);

  if (node.val == "sll" || node.val == "srl" || node.val == "sra") {
    rd = registers.at(node.args[0].val);
    rt = registers.at(node.args[1].val);
    shamt = std::stoi(node.args[2].val);
    rs = 0; // rs isn't used for shift instructions
  } else {
    rd = registers.at(node.args[0].val);
    rs = registers.at(node.args[1].val);
    rt = registers.at(node.args[2].val);
    shamt = 0; // no shamt for non-shift instruction
  }

  // Pack the fields into a 32-bit word
  uint32_t instruction = 0;
  instruction |= (opcode & 0x3F) << 26;
  instruction |= (rs     & 0x1F) << 21;
  instruction |= (rt     & 0x1F) << 16;
  instruction |= (rd     & 0x1F) << 11;
  instruction |= (shamt  & 0x1F) << 6;
  instruction |= (funct  & 0x3F);

  return instruction;
}

/**
 * @brief 
 * @param 
 * @return 
 */
uint32_t ASMInstruction::encodeIType(const ASTNode& node, uint32_t address) {
  // I-type
  /*| OPCODE |  RS   |  RT   | Immediate | */
  /*| 6-bits | 5-bit | 5-bit | 16-bits   | */

  opcode = i_type.at(node.val);
  rt = registers.at(node.args.at(0).val); // rt will always be first

  // Ex: lw $t0, 4($t2), args[1] will be 4 and not a register
  if (node.val == "beq" || node.val == "bne" || node.val == "blez" || node.val == "bgtz") {
    uint32_t offset = static_cast<uint32_t>(std::stoi(node.args.at(2).val)) - static_cast<uint32_t>(address);
    imm = static_cast<uint16_t>(offset &0xFFFF);
    rs = registers.at(node.args.at(1).val);
  } else if (registers.find(node.args.at(1).val) == registers.end()) {
    imm = std::stoi(node.args.at(1).val);
    rs = registers.at(node.args.at(2).val);
  } else {
    rs = registers.at(node.args.at(1).val);
    imm = std::stoi(node.args.at(2).val);
  }

  uint32_t instruction = 0;

  instruction |= (opcode & 0x3F) << 26;
  instruction |= (rs     & 0x1F) << 21;
  instruction |= (rt     & 0x1F) << 16;
  instruction |= (imm    & 0x1F);

  return instruction;
}

/**
 * @brief 
 * @param 
 * @return 
 */
uint32_t ASMInstruction::encodeJType(const ASTNode& node) {
  // J-type
  /*| OPCODE | Address | */
  /*| 6-bits | 26-bits | */
  opcode = j_type[node.val];
  funct = 0;
  return opcode << 26;
}

uint32_t ASMInstruction::encodePseudo(const ASTNode& node) {
  return 0;
}

bool ASMInstruction::isRType(const ASTNode& node) {
  return node.type == TokenType::INSTRUCTION && r_type.find(node.val) != r_type.end();
}

bool ASMInstruction::isIType(const ASTNode& node) {
  return node.type == TokenType::INSTRUCTION && i_type.find(node.val) != i_type.end();
}

bool ASMInstruction::isJType(const ASTNode& node) {
  return node.type == TokenType::INSTRUCTION && j_type.find(node.val) != j_type.end();
}

bool ASMInstruction::isPseudo(const ASTNode& node) {
  return node.type == TokenType::INSTRUCTION && pseudo.find(node.val) != pseudo.end();
}
