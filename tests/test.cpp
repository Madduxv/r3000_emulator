#include <cassert>
#include <cstdint>
#include <ios>
#include <iostream>
#include <ostream>
#include "test/testEncoding.hpp"
#include "test/testEmulator.hpp"

int main () {

  testEmulator();
  std::cout << "Emulator passed" << std::endl;

  uint32_t expected;
  uint32_t encoded;
  /*std::cout << "0x" << std::hex << encoded << " : " << "0x" << std::hex << expected << std::endl;*/

  /* --- R-TYPE INSTRUCTIONS --- */
  encoded = testEncoding("add $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 32;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type add passed" << std::endl;

  encoded = testEncoding("addu $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 33;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type addu passed" << std::endl;

  encoded = testEncoding("sub $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 34;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type sub passed" << std::endl;

  encoded = testEncoding("subu $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 35;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type subu passed" << std::endl;

  encoded = testEncoding("and $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 36;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type and passed" << std::endl;

  encoded = testEncoding("or $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 37;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type or passed" << std::endl;

  encoded = testEncoding("xor $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 38;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type xor passed" << std::endl;

  encoded = testEncoding("nor $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 39;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type nor passed" << std::endl;

  encoded = testEncoding("slt $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 42;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type slt passed" << std::endl;

  encoded = testEncoding("sltu $t0, $t1, $t2");
  expected = (0 << 26) | (9 << 21) | (10 << 16) | (8 << 11) | (0 << 6) | 43;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type sltu passed" << std::endl;

  encoded = testEncoding("sll $t0, $t1, 4");
  expected = (0 << 26) | (0 << 21) | (9 << 16) | (8 << 11) | (4 << 6) | 0;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type sll passed" << std::endl;

  encoded = testEncoding("srl $t0, $t1, 4");
  expected = (0 << 26) | (0 << 21) | (9 << 16) | (8 << 11) | (4 << 6) | 2;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type srl passed" << std::endl;

  encoded = testEncoding("sra $t0, $t1, 4");
  expected = (0 << 26) | (0 << 21) | (9 << 16) | (8 << 11) | (4 << 6) | 3;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type sra passed" << std::endl;

  encoded = testEncoding("jr $t1");
  expected = (0 << 26) | (9 << 21) | (0 << 16) | (0 << 11) | (0 << 6) | 8;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type jr passed" << std::endl;

  encoded = testEncoding("jalr $t0, $t1");
  expected = (0 << 26) | (9 << 21) | (0 << 16) | (8 << 11) | (0 << 6) | 9;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type jalr passed" << std::endl;

  encoded = testEncoding("syscall");
  expected = (0 << 26) | (0 << 21) | (0 << 16) | (0 << 11) | (0 << 6) | 12;
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "R-type syscall passed" << std::endl;


  /* --- I-TYPE INSTRUCTIONS --- */
  encoded = testEncoding("addi $t0, $t1, 42");
  expected = (0x08 << 26) | (9 << 21) | (8 << 16) | (42 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type addi passed" << std::endl;

  encoded = testEncoding("addiu $t0, $t1, 42");
  expected = (0x09 << 26) | (9 << 21) | (8 << 16) | (42 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type addiu passed" << std::endl;

  encoded = testEncoding("andi $t0, $t1, 0x2A");
  expected = (0x0C << 26) | (9 << 21) | (8 << 16) | (0x2A & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type andi passed" << std::endl;

  encoded = testEncoding("ori $t0, $t1, 0x2A");
  expected = (0x0D << 26) | (9 << 21) | (8 << 16) | (0x2A & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type ori passed" << std::endl;

  encoded = testEncoding("xori $t0, $t1, 0x2A");
  expected = (0x0E << 26) | (9 << 21) | (8 << 16) | (0x2A & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type xori passed" << std::endl;

  encoded = testEncoding("slti $t0, $t1, 42");
  expected = (0x0A << 26) | (9 << 21) | (8 << 16) | (42 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type slti passed" << std::endl;

  encoded = testEncoding("sltiu $t0, $t1, 42");
  expected = (0x0B << 26) | (9 << 21) | (8 << 16) | (42 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type sltiu passed" << std::endl;

  encoded = testEncoding("lui $t0, 0x1234");
  expected = (0x0F << 26) | (0 << 21) | (8 << 16) | (0x1234 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lui passed" << std::endl;

  encoded = testEncoding("lw $t0, 4($t1)");
  expected = (0x23 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lw passed" << std::endl;

  encoded = testEncoding("sw $t0, 4($t1)");
  expected = (0x2B << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type sw passed" << std::endl;

  encoded = testEncoding("lb $t0, 4($t1)");
  expected = (0x20 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lb passed" << std::endl;

  encoded = testEncoding("lbu $t0, 4($t1)");
  expected = (0x24 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lbu passed" << std::endl;

  encoded = testEncoding("lh $t0, 4($t1)");
  expected = (0x21 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lh passed" << std::endl;

  encoded = testEncoding("lhu $t0, 4($t1)");
  expected = (0x25 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type lhu passed" << std::endl;

  encoded = testEncoding("sb $t0, 4($t1)");
  expected = (0x28 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type sb passed" << std::endl;

  encoded = testEncoding("sh $t0, 4($t1)");
  expected = (0x29 << 26) | (9 << 21) | (8 << 16) | (4 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type sh passed" << std::endl;

  encoded = testEncoding("beq $t0, $t1, 4");
  expected = (0x04 << 26) | (8 << 21) | (9 << 16) | (0 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type beq passed" << std::endl;

  encoded = testEncoding("bne $t0, $t1, 4");
  expected = (0x05 << 26) | (8 << 21) | (9 << 16) | (0 & 0xFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "I-type bne passed" << std::endl;

  /* --- J-TYPE INSTRUCTIONS --- */
  encoded = testEncoding("j 0x00400000");
  expected = (0x02 << 26) | ((0x00400000 >> 2) & 0x03FFFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "J-type j passed" << std::endl;

  encoded = testEncoding("jal 0x00400000");
  expected = (0x03 << 26) | ((0x00400000 >> 2) & 0x03FFFFFF);
  // std::cout << "Exp: 0x" << std::hex << expected << " | Act: 0x" << std::hex << encoded << std::endl;
  assert(encoded == expected);
  std::cout << "J-type jal passed" << std::endl;

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
