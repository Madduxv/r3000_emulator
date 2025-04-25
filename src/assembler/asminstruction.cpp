#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/symbols.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Encodes an assembly instruction into machine-code.
 *
 * @param node      The ASTNode instruction to be encoded
 * @param address   The address of this instruction
 *
 * @return uint32_t The machine-code representation of this instruction
 */
uint32_t ASMInstruction::encode(const ASTNode node, uint32_t address) {
  if (isRType(node)) {
    return encodeRType(node);
  } else if (isIType(node)) {
    return encodeIType(node, address);
  } else if (isJType(node)) {
    return encodeJType(node);
  } else {
    std::cout << "ERROR: Unknown Instruction (" << node.val << ")" << std::endl;
    exit(1);
  }
}

/**
 * @brief Encodes an R-type instruction into a 32-bit machine code format.
 *
 * @param node      The AST node representing the current instruction.
 * @return uint32_t The encoded 32-bit machine instruction.
 */
uint32_t ASMInstruction::encodeRType(const ASTNode& node) {
  // R-type
  /*| OPCODE |  RS   |  RT   |  RD   | SHAMT | FUNCT | */
  /*| 6-bits | 5-bit | 5-bit | 5-bit | 5-bit | 6-bit | */

  if ("syscall" == node.val) {
    return 0x0000000C;
  }

  if (node.args.size() < 3) {
    std::cout << "Instruction is missing arguments: " << node.val << std::endl;
    return 0;
  }

  if (!( registers.find(node.args[0].val) != registers.end() && 
    (registers.find(node.args[1].val) != registers.end() || 
    registers.find(node.args[2].val) != registers.end()) )) {

    std::cout << "Instruction has incorrect format: " << node.val << std::endl;

  }

  opcode = 0x00;
  funct = r_type.at(node.val);

  if (node.val == "sll" || node.val == "srl" || node.val == "sra") {
    rd = registers.at(node.args[0].val);
    rt = registers.at(node.args[1].val);
    shamt = std::stoul(node.args[2].val, nullptr, 0);
    rs = 0; // rs isn't used for shift instructions
  }  else {
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
 * @brief Encodes an I-type instruction into a 32-bit machine code format.
 *
 * @param node      The AST node representing the current instruction.
 * @return uint32_t The encoded 32-bit machine instruction.
 */
uint32_t ASMInstruction::encodeIType(const ASTNode& node, uint32_t address) {
  // I-type
  /*| OPCODE |  RS   |  RT   | Immediate | */
  /*| 6-bits | 5-bit | 5-bit | 16-bits   | */

  if ("lw" != node.val && "blez" != node.val && "bgtz" != node.val && node.args.size() < 3) {
    std::cout << "Instruction is missing arguments: " << node.val << std::endl;
    return 0;
  }

  opcode = i_type.at(node.val);
  rt = registers.at(node.args.at(0).val); // rt will always be first

  // Ex: lw $t0, 4($t2), args[1] will be 4 and not a register
  try {

    if ("beq" == node.val || "bne" == node.val) {
      int32_t label_addr = std::stoul(node.args.at(2).val, nullptr, 0);  // This assumes absolute byte address
      int32_t offset = (label_addr - (address + 4)) >> 2;
      imm = static_cast<uint16_t>(offset & 0xFFFF);
      rs = registers.at(node.args.at(0).val);
      rt = registers.at(node.args.at(1).val);

    } else if ("blez" == node.val || "bgtz" == node.val) {
      int32_t label_addr = std::stoul(node.args.at(1).val, nullptr, 0);
      int32_t offset = (label_addr - (address + 4)) >> 2;
      imm = static_cast<uint16_t>(offset & 0xFFFF);
      rs = registers.at(node.args.at(0).val);
      rt = 0;

    } else if (registers.find(node.args.at(1).val) == registers.end()) {
      imm = std::stoul(node.args.at(1).val, nullptr, 0);
      rs = registers.at(node.args.at(2).val);

    } else if ("lw" == node.val || "lui" == node.val) {
      imm = std::stoul(node.args.at(1).val, nullptr, 0);
      rs = 0;

    } else {
      rs = registers.at(node.args.at(1).val);
      imm = std::stoul(node.args.at(2).val, nullptr, 0);
    }

  } catch (std::out_of_range) {
    std::cout << "Unknown registers for instruction (" << node << ")"<< std::endl;
  }

  uint32_t instruction = 0;

  instruction |= (opcode & 0x3F) << 26;
  instruction |= (rs     & 0x1F) << 21;
  instruction |= (rt     & 0x1F) << 16;
  instruction |= (imm    & 0xFFFF);

  return instruction;
}

/**
 * @brief Encodes an J-type instruction into a 32-bit machine code format.
 *
 * @param node      The AST node representing the current instruction.
 * @return uint32_t The encoded 32-bit machine instruction.
 */
uint32_t ASMInstruction::encodeJType(const ASTNode& node) {
  // J-type
  /*| OPCODE | Address | */
  /*| 6-bits | 26-bits | */

  if (node.args.size() < 1) {
    std::cout << "Instruction is missing arguments: " << node.val << std::endl;
    return 0;
  }

  opcode = j_type.at(node.val);
  
  uint32_t targetAddr = std::stoul(node.args[0].val, nullptr, 0); // should be pre-resolved

  // word-align and fit to 26 bits (even though I don't support full 26-bit addresses yet)
  addr = (targetAddr >> 2) & 0x03FFFFFF;

  return (opcode << 26) | addr;
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
