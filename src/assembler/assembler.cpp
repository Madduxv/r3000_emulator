#include "assembler/assembler.hpp"
#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/parser.hpp"
#include "assembler/symbols.hpp"
#include "emulator/memory.hpp"
#include <cstdint>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>

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

Assembler::Assembler(const std::string& fileName, Memory& mem): varAddrPtr(0x5000) {
  /*ASMInstruction encoder;*/
  std::string file = readFile("testFile.s");
  std::vector<Token> tokenizedFile = tokenize(file);
  this->AST = parse(tokenizedFile);
  getSymbols(this->AST, mem);
  resolveSymbols(this->AST);
  this->instrAddrPtr = setStart(mem);

  /*for (const ASTNode& node : this->AST) {*/
  /*  if (node.type == TokenType::INSTRUCTION) { */
  /*    this->Instructions.push_back(encoder.encode(node, instrAddrPtr)); */
  /*    this->instrAddrPtr += 4;*/
  /*  }*/
  /*}*/

  if (this->instrAddrPtr == 0xFFFFFFFF) {
    std::cout << "ERROR: No start address present" << std::endl;
    exit(1);
  }

  for (const ASTNode& node : this->AST) {
    std::cout << node << '\n';
  }

}


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


void Assembler::assemble(Memory& mem) {
}
