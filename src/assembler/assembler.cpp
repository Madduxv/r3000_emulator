#include "assembler/assembler.hpp"
#include "assembler/parser.hpp"
#include "assembler/symbols.hpp"
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

Assembler::Assembler(const std::string& fileName): varAddrPtr(0x5000) {

	std::string file = readFile("testFile.s");
	std::vector<Token> tokenizedFile = tokenize(file);
  this->AST = parse(tokenizedFile);
  getSymbols(this->AST);
  resolveSymbols(this->AST);
  this->instrAddrPtr = setStart();

  if (this->instrAddrPtr == 0xFFFFFFFF) {
    std::cout << "ERROR: No start address present" << std::endl;
    exit(1);
  }

	for (const ASTNode& node : this->AST) {
		std::cout << node << '\n';
	}

}

uint32_t Assembler::encodeRType(const ASTNode& node) {
  return 0;
}

uint32_t Assembler::encodeIType(const ASTNode& node) {
  return 0;
}

uint32_t Assembler::encodeJType(const ASTNode& node) {
  return 0;
}

// I'll add pseudo-instructions last
uint32_t Assembler::encodePseudo(const ASTNode& node) {
  return 0;
}

void Assembler::insertVariable(const ASTNode& node) {

}

void Assembler::insertInstr(uint32_t addr, uint32_t instr) {

}

uint32_t Assembler::setStart() {
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


std::vector<uint8_t> Assembler::assemble() {
  return {};
}
