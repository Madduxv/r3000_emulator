
#include <cstdint>
#include <iostream>
#include <ostream>
#include "emulator/instructions.hpp"

/**
 * @brief Decodes a machine-code instruction and extracts the different sections of the instruction
 *
 * @param instr         The instruction to be decoded
 * @return Instruction  The decoded instruction
 */
	Instruction Instruction::decode(uint32_t instr) {
		Instruction i;
		i.opcode = (instr >> 26) & 0x3F;
		i.rs = (instr >> 21) & 0x1F;
		i.rt = (instr >> 16) & 0x1F;

		if (i.opcode == 0x0) {  // R-type
			i.rd = (instr >> 11) & 0x1F;
			i.shamt = (instr >> 6) & 0x1F;
			i.funct = instr & 0x3F;
		} else if (i.opcode == 0x2 || i.opcode == 0x3) {  // J-type
			i.addr = instr & 0xFFFF;
		} else {  // I-type
			i.imm = instr & 0x3FFFFFF;
		}

		return i;
	}

/**
 * @brief 
 * @param 
 * @return 
 */
	void Instruction::print() {
		Instruction i;
		std::cout << "Opcode: 0x" << std::hex << (int)i.opcode << std::endl;
		std::cout << "RS:     0x" << std::hex << (int)i.rs << std::endl;
		std::cout << "RT:     0x" << std::hex << (int)i.rd << std::endl;

		if (i.opcode == 0x0) {  // R-type
			std::cout << "RD:     0x" << std::hex << (int)i.rt << std::endl;
			std::cout << "Shamt:  0x" << std::hex << (int)i.shamt << std::endl;
			std::cout << "Funct:  0x" << std::hex << (int)i.funct << std::endl;
		} else if (i.opcode == 0x2 || i.opcode == 0x3) {  // J-type
			std::cout << "Addr:   0x" << std::hex << i.addr << std::endl;
		} else {  // I-type
			std::cout << "Imm:    0x" << std::hex << i.imm << std::endl;
		}
		std::cout << std::endl;
	}

