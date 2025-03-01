/*#include <fstream>*/
/*#include <cstdint>*/
/*#include <iostream>*/
/*#include <map>*/
/*#include <ostream>*/
/*#include <string>*/
/*#include <vector>*/


/*struct Memory {*/
/* Memory regions:
* 0x0000 - 0x4FFF - ROM/Program
* 0x4FFC - 0x4FFD - Reset Vector
* 0x5000 - 0x5FFF - Chars (.data)
* 0x6000 - 0x7FFF - Variables/Heap
* 0x7000 - 0x9FFF - Stack
* 0x9000 - 0x9FFF - Peripherals (stdout or whatever I add)
* 0xA000 - 0xFFFF - Unused for now


static const uint32_t MEM_SIZE = 0x10000;
std::vector<uint8_t> mem;

public:
Memory() : mem(MEM_SIZE, 0) {}
uint8_t read8(uint32_t address) { return mem[address & (MEM_SIZE - 1)]; }
uint16_t read16(uint32_t address) { return (read8(address) << 8) | read8((address + 1) & (MEM_SIZE - 1)); }
uint32_t read32(uint32_t address) { return (read16(address) << 16) | read16((address + 2) & (MEM_SIZE - 1)); }

void write8(uint32_t address, uint8_t value) { mem[address & (MEM_SIZE - 1)] = value; }
void write16(uint32_t address, uint16_t value) {
	write8(address, value >> 8);
	write8((address + 1) & (MEM_SIZE - 1), value & 0xFF);
}
void write32(uint32_t address, uint32_t value) {
	write16(address, value >> 16);
	write16((address + 2) & (MEM_SIZE - 1), value & 0xFFFF);
}
};

Registers:*/
/**/
/*	0: zero reg*/
/*	1: at (for assembler don't worry about it)*/
/*	2 - 3:	funct return values*/
/*	4 - 7:	funct args*/
/*	8 - 9 / 24 - 25:temp / saved (caller)*/
/*	16 - 23:saved (callee)*/
/*	26 - 27:for a kernal (lol no)*/
/*	28:global pointer*/
/*	29:stack pointer*/
/*	30:frame reg or another saved reg*/
/*	31:return address*/



/*const std::map<std::string, int> Register_Index; */
/*= { {"zero", 0}, {"at", 1}, {"v0", 2}, {"v1", 3}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, };*/


// gives [type, opcode, shamt/imm(1,2), num registers, funct]
/*const std::map<std::string, int> opcode; */
/*= { {"addi", 0x08}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, */
/*{}, {}, {}, {}, {}, {}, {}, {}, };*/

/*struct decodedInstruction {*/
/*	uint8_t opcode;*/
/*	uint8_t rs;*/
/*	uint8_t rt;*/
/*	uint8_t rd;     // Only for R-type*/
/*	uint8_t shamt;  // Only for R-type*/
/*	uint8_t funct;  // Only for R-type*/
/*	uint16_t imm;   // Only for I-type*/
/*	uint32_t addr;  // Only for J-type*/
/**/
/**/
/*};*/
/**/
/*void assemble(std::string filename) {*/
/*	std::vector<std::string> lines;*/
/*	std::vector<std::string> tokenizedLine;*/
/*	uint32_t instruction;*/
/**/
/*	std::string line;*/
/*	std::ifstream myfile (filename);*/
/*	if (myfile.is_open()) {*/
/*		while (getline(myfile, line) ) {*/
/*			std::cout << line << std::endl;*/
/*		}*/
/*		myfile.close();*/
/*	}*/
/**/
/*	else std::cout << "Unable to open file"; */
/**/
/*}*/
/**/
