#include "assembler/assembler.hpp"
#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/parser.hpp"
#include "assembler/symbols.hpp"
#include "emulator/instructions.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <ios>
#include <iostream>
#include <ostream>
#include <fstream>
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
  ASMInstruction encoder;
  for (const ASTNode& node : this->AST) {
    if (node.type == TokenType::INSTRUCTION) { 
      uint32_t encoded = encoder.encode(node, instrAddrPtr);
      this->Instructions.push_back(encoded); 
      mem.write32(this->instrAddrPtr, encoded);
      this->instrAddrPtr += 4;
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
