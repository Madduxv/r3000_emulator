#include "assembler/asminstruction.hpp"
#include "assembler/lexer.hpp"
#include "assembler/parser.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


uint32_t testEncoding(const std::string input) {
  ASMInstruction encoder;
  std::vector<Token> tokens = tokenize(input);
  std::vector<ASTNode> nodes = parse(tokens);
  uint32_t encoded = encoder.encode(nodes[0], 0);


  // debug prints--------------------
  /*for(const ASTNode node : nodes) {*/
  /*  std::cout << node << std::endl;*/
  /*}*/
  /*std::cout << encoded << std::endl;*/
  // --------------------------------

  return encoded;
}

int main () {
  uint32_t expected;
  uint32_t encoded;
  /*std::cout << "0x" << std::hex << encoded << " : " << "0x" << std::hex << expected << std::endl;*/

  // --- R-TYPE ---
  encoded = testEncoding("add $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 32;
  assert(encoded == expected);
  std::cout << "R-type add passed" << std::endl;

  encoded = testEncoding("sub $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 34;
  assert(encoded == expected);
  std::cout << "R-type sub passed" << std::endl;

  encoded = testEncoding("and $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 36;
  assert(encoded == expected);
  std::cout << "R-type and passed" << std::endl;

  encoded = testEncoding("or $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 37;
  assert(encoded == expected);
  std::cout << "R-type or passed" << std::endl;

  encoded = testEncoding("slt $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 42;
  assert(encoded == expected);
  std::cout << "R-type slt passed" << std::endl;

  // --- I-TYPE ---
  encoded = testEncoding("addi $t0, $t1, 42");
  expected = (0x08 << 26) | (9 << 21) | (8 << 16) | (42 & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type addi passed" << std::endl;

  encoded = testEncoding("andi $t0, $t1, 0x2A");
  expected = (0x0C << 26) | (9 << 21) | (8 << 16) | (0x2A & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type andi passed" << std::endl;

  encoded = testEncoding("ori $t0, $t1, 0x2A");
  expected = (0x0D << 26) | (9 << 21) | (8 << 16) | (0x2A & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type ori passed" << std::endl;

  encoded = testEncoding("lw $t0, 4($t1)");
  expected = (0x23 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type lw passed" << std::endl;

  encoded = testEncoding("sw $t0, 4($t1)");
  expected = (0x2B << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type sw passed" << std::endl;

  encoded = testEncoding("beq $t0, $t1, 4");
  expected = (0x04 << 26) | (8 << 21) | (9 << 16) | (0 & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type beq passed" << std::endl;

  encoded = testEncoding("bne $t0, $t1, 4");
  expected = (0x05 << 26) | (8 << 21) | (9 << 16) | (0 & 0xFFFF);
  assert(encoded == expected);
  std::cout << "I-type bne passed" << std::endl;

  // --- J-TYPE ---
  encoded = testEncoding("j 0x00400000");
  expected = (0x02 << 26) | ((0x00400000 >> 2) & 0x03FFFFFF);
  assert(encoded == expected);
  std::cout << "J-type j passed" << std::endl;

  encoded = testEncoding("jal 0x00400000");
  expected = (0x03 << 26) | ((0x00400000 >> 2) & 0x03FFFFFF);
  assert(encoded == expected);
  std::cout << "J-type jal passed" << std::endl;

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
