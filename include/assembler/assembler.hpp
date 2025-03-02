#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <cstdint>
#include <string>
#include <vector>
struct Assembler {
  std::vector<uint8_t> machineCode;
  uint32_t* asciiAddrPtr;
  uint32_t* intAddrPtr;

  static uint32_t encodeRType(uint8_t opcode, uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);
  static uint32_t encodeIType(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t imm);
  static uint32_t encodeJType(uint8_t opcode, uint32_t address);
  static void insertGlobal();

public:
  Assembler();
  std::vector<uint8_t> assemble(std::string filename);
};



#endif // !ASSEMBLER_HPP
