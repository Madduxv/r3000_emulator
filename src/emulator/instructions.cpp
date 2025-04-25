#include <cstdint>
#include <iostream>
#include <ostream>
#include "emulator/instructions.hpp"

/**
 * @brief Decodes a machine-code instruction and extracts the different sections of the instruction.
 *
 * @param instr         The instruction to be decoded.
 * @return Instruction  The decoded instruction.
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
 * @brief Prints the different "arguments" of an instruction.
 */
	void Instruction::print() {
		std::cout << "Opcode: 0x" << std::hex << (int)this->opcode << std::endl;
		std::cout << "RS:     0x" << std::hex << (int)this->rs << std::endl;
		std::cout << "RT:     0x" << std::hex << (int)this->rd << std::endl;

		if (this->opcode == 0x0) {  // R-type
			std::cout << "RD:     0x" << std::hex << (int)this->rt << std::endl;
			std::cout << "Shamt:  0x" << std::hex << (int)this->shamt << std::endl;
			std::cout << "Funct:  0x" << std::hex << (int)this->funct << std::endl;
		} else if (this->opcode == 0x2 || this->opcode == 0x3) {  // J-type
			std::cout << "Addr:   0x" << std::hex << this->addr << std::endl;
		} else {  // I-type
			std::cout << "Imm:    0x" << std::hex << this->imm << std::endl;
		}
		std::cout << std::endl;
	}

