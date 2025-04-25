#include "assembler/assembler.hpp"
#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/parser.hpp"
#include "assembler/symbols.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/**
 * @brief Reads a file and returns its contents as a string
 *
 * @param filename      The path to a file.
 * @return std::string  The contents of the file as a string.
 */
std::string readFile(const std::string& filename) {
  std::ifstream myfile(filename);
  if (!myfile.is_open()) {
    std::cout << "Unable to open file\n";
    return "";
  }

  std::string file;
  std::string line;
  while (getline(myfile, line)) {
    file.append(line + '\n');  // Preserve line breaks
  }
  myfile.close();
  return file;
}

/**
 * @brief Creates a new Assembler object with its fields defined.
 *
 * @param fileName  The path to the assembly file as a string.
 * @param mem       A reference to the Memory object that will hold the machine-code for the program.
 * @return          A new Assembler object.
 */
Assembler::Assembler(const std::string& fileName, Memory& mem): varAddrPtr(0x5000) {
  std::string file = readFile("testFile.s");
  std::vector<Token> tokenizedFile = tokenize(file);
  this->AST = parse(tokenizedFile);
  getSymbols(this->AST, mem);
  resolveSymbols(this->AST);
  this->instrAddrPtr = setStart(mem);

  if (this->instrAddrPtr == 0xFFFFFFFF) {
    std::cout << "ERROR: No start address present" << std::endl;
    exit(1);
  }

}


/**
 * @brief Gets the start address for the assembly program.
 *
 * @param mem       A reference to the memory object that will house the machine code.
 * @return uint32_t The address for the start of the program.
 */
uint32_t Assembler::setStart(Memory& mem) {
  uint32_t startAddr;
  for(const ASTNode& node : this->AST) {
    if (node.val == ".globl") {
      startAddr = static_cast<uint32_t>(std::stoul(node.args.at(0).val));
      mem.write16(0x4FFC, startAddr);
      return startAddr;
    }
  }
  return 0xFFFFFFFF;
}


/**
 * @brief Turns the abstract syntax tree into machine code and writes it to the memory object.
 *
 * @param mem The memory object that will contain the machine-code
 */
void Assembler::assemble(Memory& mem) {
  for (const ASTNode& node : this->AST) {
    if (node.type == TokenType::INSTRUCTION) { 
      if (isPseudo(node)) {
        encodePseudo(node, mem);
      } else {
        uint32_t encoded = encoder.encode(node, instrAddrPtr);
        this->Instructions.push_back(encoded); 
        mem.write32(this->instrAddrPtr, encoded);
        this->instrAddrPtr += 4;
      }
    }
  }
}

/**
 * @brief Prints the contents of the abstract syntax tree created from the given assembly program.
 */
void Assembler::print() {
  std::cout << "Start address: 0x" << std::hex << this->instrAddrPtr << std::endl;
  for (const ASTNode& node : this->AST) {
    std::cout << node << '\n';
  }
}

/**
 * @brief Checks if an instruction is a pseudo-instruction.
 *
 * @param node      A reference to the node to be checked.
 * @return bool     True if the instruction is a pseudo-instruction and false otherwise.
 */
bool isPseudo(const ASTNode node) {
  return pseudo.find(node.val) != pseudo.end();
}

/**
 * @brief Expands a pseudo-instruction into its real counterparts.
 *
 * @param node      A reference to the node to be converted.
 * @param mem       A reference to the Memory object that will store the machine-code.
 */
void Assembler::encodePseudo(const ASTNode node, Memory& mem) {

  std::stringstream ss("");

  if (node.val == "move") {
    // move $reg1, $reg2 => add $reg1, $reg2, $zero
    if (node.args.size() != 2) {
      std::cout << "Not enough arguments for instruction: move" << std::endl;
      exit(1);
    }

    ss << "add " << node.args.at(0).val << ", " 
      << node.args.at(1).val << ", " << "$zero";

    encodeNewNodes(ss.str(), mem);
  }

  else if (node.val == "li") {

    if (node.args.size() != 2) {
      std::cout << "Not enough arguments for instruction: li" << std::endl;
      exit(1);
    }

    // li $reg, imm -> ori $reg, $zero, imm (simple case)
    // For large imm split into lui + ori
    uint32_t imm = std::stoul(node.args.at(1).val, nullptr, 0);

    if (imm <= 0xFFFF) {
      ss << "ori " << node.args.at(0).val << ", $zero, " 
        << node.args.at(1).val;

      encodeNewNodes(ss.str(), mem);

    } else {
      // lui + ori
      uint32_t upper = (imm >> 16) & 0xFFFF;
      uint32_t lower = imm & 0xFFFF;
      const std::string& reg = node.args.at(0).val;

      std::stringstream ss_lui;
      std::stringstream ss_ori;

      ss_lui << "lui " << reg << ", " << upper;
      ss_ori << "ori " << reg << ", " << reg << ", " << lower;

      encodeNewNodes(ss.str(), mem);
      encodeNewNodes(ss.str(), mem);
    }
  }
  else if (node.val == "nop") {
    // nop => sll $zero, $zero, 0
    ss << "sll $zero, $zero, 0";
    encodeNewNodes(ss.str(), mem);
  } else if (node.val == "la") {
    ss << "ori" << node.args.at(0).val << ",  $zero, " << node.args.at(1).val;
    encodeNewNodes(ss.str(), mem);
  }
  else {
    // How did we get here?
    throw std::runtime_error("Unknown pseudo-instruction: " + node.val);
  }
}


/**
 * @brief Encodes a new instruction "on the fly" into machine code
 *
 * @param str       The new instruction to be encoded
 * @param mem       A reference to the Memory object that will store the machine-code.
 */
void Assembler::encodeNewNodes(std::string str, Memory& mem) {
  std::vector<Token> tokens = tokenize(str);
  ASTNode newNode = parse(tokens).at(0);
  uint32_t encoded = encoder.encode(newNode, this->instrAddrPtr);
  this->Instructions.push_back(encoded);
  mem.write32(this->instrAddrPtr, encoded);
  this->instrAddrPtr+=4;
}
