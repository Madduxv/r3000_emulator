#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

struct Memory {

	/* Memory regions:
 *
 * 0x0000 - 0x4FFF - ROM/Program
 * 0x4FFC - 0x4FFD - Reset Vector
 * 0x5000 - 0x5FFF - Chars (.data)
 * 0x6000 - 0x7FFF - Variables/Heap
 * 0x7000 - 0x9FFF - Stack
 * 0x9000 - 0x9FFF - Peripherals (stdout or whatever I add)
 * 0xA000 - 0xFFFF - Unused for now
 *
 */

	static const uint32_t MEM_SIZE = 0x10000; // Yes only 64kb
	std::vector<uint8_t> mem;

public:
	Memory() : mem(MEM_SIZE, 0) {}

	uint8_t read8(uint32_t address) {
		return mem[address & (MEM_SIZE - 1)];
	}

	uint16_t read16(uint32_t address) {
		return (read8(address) << 8) | read8((address + 1) & (MEM_SIZE - 1));
	}

	uint32_t read32(uint32_t address) {
		return (read16(address) << 16) | read16((address + 2) & (MEM_SIZE - 1));
	}

	void write8(uint32_t address, uint8_t value) {
		mem[address & (MEM_SIZE - 1)] = value;
	}

	void write16(uint32_t address, uint16_t value) {
		write8(address, value >> 8);
		write8((address + 1) & (MEM_SIZE - 1), value & 0xFF);
	}

	void write32(uint32_t address, uint32_t value) {
		write16(address, value >> 16);
		write16((address + 2) & (MEM_SIZE - 1), value & 0xFFFF);
	}

};

struct CPU {
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
	uint32_t registers[32]; 
public:
	uint32_t pc = 0;
	uint32_t hi = 0, lo = 0;
	// HI/LO registers for multiplication/division, which I will totally add

	CPU() : pc(0), hi(0), lo(0) {
		memset(registers, 0, sizeof(registers));
		registers[29] = registers[30] = 0x9FFE;
		// Beginning of the stack
	}

	uint32_t getRegister(int index) {
		return registers[index & 31];
	}

	void setRegister(int index, uint32_t value) {
		if (index != 0) {registers[index & 31] = value;}
	}
	
	void print() {
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
};

struct Instruction {

	/* Register names
	 *
	 * rs -		 Source Register
	 * rt -		 Target Register
	 * rd -		 Destination Register
	 * imm -	 Immediate
	 * shamt - Shift Amount
	 *
	 */

	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;     // Only for R-type
	uint8_t shamt;  // Only for R-type
	uint8_t funct;  // Only for R-type
	uint16_t imm;   // Only for I-type
	uint32_t addr;  // Only for J-type

	static Instruction decode(uint32_t instr) {
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

	void print() {
		std::cout << "Opcode: 0x" << std::hex << (int)opcode << std::endl;
		std::cout << "RS:     0x" << std::hex << (int)rs << std::endl;
		std::cout << "RT:     0x" << std::hex << (int)rd << std::endl;

		if (opcode == 0x0) {  // R-type
			std::cout << "RD:     0x" << std::hex << (int)rt << std::endl;
			std::cout << "Shamt:  0x" << std::hex << (int)shamt << std::endl;
			std::cout << "Funct:  0x" << std::hex << (int)funct << std::endl;
		} else if (opcode == 0x2 || opcode == 0x3) {  // J-type
			std::cout << "Addr:   0x" << std::hex << addr << std::endl;
		} else {  // I-type
			std::cout << "Imm:    0x" << std::hex << imm << std::endl;
		}
		std::cout << std::endl;
	}
};

void startup(Memory& mem, CPU &cpu) {
	// Our reset vector will be at FFFC for now
	cpu.pc = 0x00000000 + mem.read16(0x4FFC); // Yes, we are using big endian
}

// I really should have just used the 6502 instruction set lol
void execute(Instruction &instr, CPU &cpu, Memory &mem) {
	// yes I know the pc is iterated after, but this is my emulator
	// i can do whatever I want
	cpu.pc = ( cpu.pc + 4 ) & 0xFFFF;

	// R-type
	if (instr.opcode == 0x00) {
		switch (instr.funct) {

			case 0x0c: // SYSCALL
				// v0: syscall type
				// a0-2: args
				if (cpu.getRegister(2) == 4) { // syswrite
					for (int i = 0; i < cpu.getRegister(6); i++) {
						std::cout << char(mem.read8(cpu.getRegister(5) + i));
					}
				} else if (cpu.getRegister(2) == 10) { // exit
					std::exit(0);
				}
				break;

			case 0x00: // SLL
				break;

			case 0x02: // SRL
				break;

			case 0x03: // SRA
				break;

			case 0x08: // JR
				cpu.pc = cpu.getRegister(instr.rs) & 0xFFFF;
				// I don't think i'll byte align jump instructions
				break;

			case 0x09: // JALR
				break;

			case 0x18: // MULT
				cpu.hi = static_cast<uint32_t>((static_cast<uint64_t>(cpu.getRegister(instr.rs)) * 
																	 static_cast<uint64_t>(cpu.getRegister(instr.rt))) >> 32);

				cpu.lo = static_cast<uint32_t>((static_cast<uint64_t>(cpu.getRegister(instr.rs)) * 
																	 static_cast<uint64_t>(cpu.getRegister(instr.rt))));
				break;

			case 0x19: // MULTU
				cpu.hi = static_cast<uint32_t>((static_cast<uint64_t>(cpu.getRegister(instr.rs)) * 
																	 static_cast<uint64_t>(cpu.getRegister(instr.rt))) >> 32);

				cpu.lo = static_cast<uint32_t>((static_cast<uint64_t>(cpu.getRegister(instr.rs)) * 
																	 static_cast<uint64_t>(cpu.getRegister(instr.rt))));
				break;

			case 0x1a: // DIV
				cpu.lo = cpu.getRegister( instr.rs ) / cpu.getRegister( instr.rt ); 
				cpu.hi = cpu.getRegister( instr.rs ) % cpu.getRegister( instr.rt );
				break;

			case 0x1b: // DIVU
				cpu.lo = cpu.getRegister( instr.rs ) / cpu.getRegister( instr.rt ); 
				cpu.hi = cpu.getRegister( instr.rs ) % cpu.getRegister( instr.rt );
				break;

			case 0x20: // ADD
				cpu.setRegister(instr.rd, cpu.getRegister(instr.rs) + cpu.getRegister(instr.rt));
				break;

			case 0x21: // ADDU
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) + cpu.getRegister( instr.rt ));
				break;

			case 0x22: // SUB
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) - cpu.getRegister( instr.rt ));
				break;

			case 0x23: // SUBU
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) - cpu.getRegister( instr.rt ));
				break;

			case 0x24: // AND
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) & cpu.getRegister( instr.rt ));
				break;

			case 0x25: // OR
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) | cpu.getRegister( instr.rt ));
				break;

			case 0x26: // XOR
				cpu.setRegister(instr.rd, cpu.getRegister( instr.rs ) ^ cpu.getRegister( instr.rt ));
				break;

			case 0x27: // NOR
				cpu.setRegister(instr.rd, ~(cpu.getRegister( instr.rs ) | cpu.getRegister( instr.rt )));
				break;

			case 0x2a: // SLT
				break;

			case 0x2b: // SLTU
				break;

		}

	}
	// J-type
	switch (instr.opcode) {
		case 0x02: // J
			cpu.setRegister(31, cpu.pc);
			cpu.pc = instr.addr & 0xFFFF; // haven't increased memory space yet
			// not sure if I want jumps byte aligned
			/*std::cout << "New PC: " << cpu.pc << std::endl;*/
			break;

		case 0x03: // JAL
			mem.write16(cpu.getRegister(29), cpu.pc);
			// TODO: Impl bounds check
			cpu.setRegister(29, ( cpu.getRegister(29) - 4 ));
			cpu.pc = instr.addr & 0xFFFF;
			break;

		// I-type
		case 0x04: // BEQ
			if (cpu.getRegister(instr.rs) == cpu.getRegister(instr.rt)) {
				cpu.setRegister(31, cpu.pc);
				cpu.pc += (static_cast<int16_t>(instr.imm) << 2);
				// I don't feel like implementing the delay slot yet
				// Branches will be byte aligned here
			}
			break;

		case 0x05: // BNE
			if (cpu.getRegister(instr.rs) != cpu.getRegister(instr.rt)) {
				cpu.setRegister(31, cpu.pc);
				cpu.pc += (static_cast<int16_t>(instr.imm) << 2);
			}
			break;

		case 0x06: // BLEZ
			if ((int32_t)cpu.getRegister(instr.rs) <= 0) {
				cpu.setRegister(31, cpu.pc);
				cpu.pc += (static_cast<int16_t>(instr.imm) << 2);
			}
			break;

		case 0x07: // BGTZ
			if ((int32_t)cpu.getRegister(instr.rs) > 0) {
				cpu.setRegister(31, cpu.pc);
				cpu.pc += ( static_cast<int16_t>(instr.imm) << 2);
			}
			break;

		case 0x08: // ADDI
			cpu.setRegister(instr.rt, instr.imm + cpu.getRegister(instr.rs));
			// I'll handle signed and unsigned later
			break;

		case 0x09: // ADDIU
			cpu.setRegister(instr.rt, instr.imm + cpu.getRegister(instr.rs));
			break;

		case 0x0A: // SLTI
			cpu.setRegister(instr.rt, cpu.getRegister(instr.rs) << instr.imm );
			break;

		case 0x0B: // SLTUI
			break;

		case 0x0C: // ANDI
			cpu.setRegister(instr.rt, instr.imm & cpu.getRegister(instr.rs));
			break;

		case 0x0D: // ORI
			cpu.setRegister(instr.rt, instr.imm | cpu.getRegister(instr.rs));
			break;

		case 0x0E: // XORI
			cpu.setRegister(instr.rt, instr.imm ^ cpu.getRegister(instr.rs));
			break;

		case 0x0F: // LUI
			break;

		case 0x20: // LB
			break;

		case 0x21: // LH
			break;

		case 0x23: // LW
			break;

		case 0x24: // LBU
			break;

		case 0x25: // LHU
			break;

		case 0x28: // SB
			break;

		case 0x29: // SH
			break;

		case 0x2b: // SW
			break;

	}

}


void run(Memory &mem, CPU &cpu) {
	Instruction instr; 

	do {
		instr = Instruction::decode(mem.read32(cpu.pc));
		/*instr.print();*/
		/*std::cout << std::hex << cpu.getRegister(0xB) << ", ";*/
		/*std::cout << std::hex << cpu.getRegister(0xC) << std::endl;*/
		execute(instr, cpu, mem);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	/*} while (true);*/
	} while (!(mem.read32(cpu.pc) == 0x0000000C && cpu.getRegister(2) == 0x0A));

}



int main () {
	Memory mem;
	CPU cpu;

	// J-type
	/*| OPCODE | Address | */
	/*| 6-bits | 26-bits | */
	// 000010 000000000000000000 1111 0000
	// 0000 1000 0000 0000 0000 0000 1111 0000

	// R-type
	/*| OPCODE |  RS   |  RT   |  RD   | SHAMT | FUNCT | */
	/*| 6-bits | 5-bit | 5-bit | 5-bit | 5-bit | 6-bit | */
	// rd = rs-rt
	// 000000 01010 01011 01010 00000 100010 
	// 0000 0001 0100 1011 0101 0000 0010 0010 

	// I-type
	/*| OPCODE |  RS   |  RT   | Immediate | */
	/*| 6-bits | 5-bit | 5-bit | 16-bits   | */
	// 000111 01011 00000 1111 1111 1111 0100
	// 0001 1101 0110 0000 1111 1111 1111 1101

	mem.write16(0x4FFC, 0x0000);			// glob main
	uint32_t prgm[] = {
		0x2008002A, // li t1, #0x2a
		0x20090001, // li t2, #0x01
		0x01095020, // add t3, t1, t2

		0x20020004, // li v0, 4 (SYS_WRITE)
		0x20055000, // li a1, 0x5000 (buffer addr)
		0x2006000E, // li a2, 14 (len)

		0x200B0004, // li t3, #4
		0x200C0001, // li t4, #1
		0x0000000C, // syscall
		0x016C5822, // sub t3, t3, t4
		0x1D60FFFD, // bgtz, t3, #-3
		0x00000000, // noop
		0x080000F0, // J end
	};
	mem.write32(0x00F0, 0x2002000A);	// li v0, $10 (exit)
	mem.write32(0x00F4, 0x0000000C);	// syscall
	/*mem.write32(0x00F4, 0x080000F0);	// J end*/

	//.data
	std::string message = "Hello, World!\n\0";
	int idx = 0;
	for (char letter : message) {
		mem.write8(0x5000+(idx++), int(letter));
	}

	// Compile?
	idx = 0;
	for(uint32_t instruction : prgm) {
		mem.write32(idx, instruction);
		idx+=4;
	}

	startup(mem, cpu);
	run(mem, cpu);

	/*cpu.print();*/

	return 0;
}
