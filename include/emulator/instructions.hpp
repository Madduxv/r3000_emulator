#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>

struct Instruction {

	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint16_t imm;
	uint32_t addr;
 
public:
	static Instruction decode(uint32_t instr);
	void print();
};

#endif
