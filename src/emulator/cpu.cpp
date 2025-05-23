
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include "emulator/cpu.hpp"

/*Registers:
	 *
	 * 0:									zero reg
	 * 1:									at (for assembler don't worry about it)
	 * 2 - 3:							funct return values
	 * 4 - 7:							funct args
	 * 8 - 9 / 24 - 25:		temp / saved (caller)
	 * 16 - 23:						saved (callee)
	 * 26 - 27:						for a kernal (lol no)
	 * 28:								global pointer
	 * 29:								stack pointer
	 * 30:								frame reg or another saved reg
	 * 31:								return address
	 *
	 */

/**
 * @brief Creates a new CPU object and initializes all of its registers.
 *
 * The default constructor for the CPU struct sets all of the 
 * registers to 0, except the frame pointer, which gets set to 
 * the address 0xBFFF, which is the start of the stack.
 */
CPU::CPU() : pc(0), hi(0), lo(0) {
	memset(CPU::registers, 0, sizeof(CPU::registers));
	CPU::registers[29] = CPU::registers[30] = 0xBFFF; 
	// fp to the beginning of the stack
}

/**
 * @brief Gets the value in the given register.
 *
 * @param index     The register to retrieve the value of.
 * @return uint32_t The value in the given register.
 */
uint32_t CPU::getRegister(int index) {
	return CPU::CPU::registers[index & 31];
}

/**
 * @brief Sets the given register to the given value.
 *
 * @param index The index of the register to set to the given value.
 * @param value The value to set the register to.
 */
void CPU::setRegister(int index, uint32_t value) {
	if (index != 0) {registers[index & 31] = value;}
}

/**
 * @brief Prints the values in each register.
 */
void CPU::print() {
	std::cout << "Zero: " << registers[0] << std::endl;
	std::cout << "AT:   " << registers[1] << std::endl;
	std::cout << "v0:   " << registers[2] << std::endl;
	std::cout << "v1:   " << registers[3] << std::endl;

	int idx = 4;
	std::cout << "a0-3: ";
	while (idx < 7) {
		std::cout << registers[idx++] << " ";
	}

	std::cout << std::endl << "t0-9: ";
	while (idx < 15) {
		std::cout << registers[idx++] << " ";
	}
	std::cout << registers[24] << " " << registers[25] << std::endl;

	std::cout << "s0-7: ";
	while (idx < 23) {
		std::cout << registers[idx++] << " ";
	}

	std::cout << std::endl << "k0-1: " << registers[26] << " " << registers[27] << std::endl;
	std::cout << "gp:   " << registers[28] << std::endl;
	std::cout << "sp:   " << std::hex << registers[29] << std::endl;
	std::cout << "fp:   " << std::hex << registers[30] << std::endl;
	std::cout << "pc:   " << std::hex << pc << std::endl;
	std::cout << "ra:   " << std::hex << registers[31] << std::endl;
}

