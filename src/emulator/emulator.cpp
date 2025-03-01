#include "emulator/emulator.hpp"
#include "emulator/memory.hpp"
#include "emulator/cpu.hpp"
#include "emulator/instructions.hpp"

#include <iostream>
#include <thread>

void startup(Memory& mem, CPU &cpu) {
	// Our reset vector will be at 4FFC for now
	cpu.pc = 0x00000000 + mem.read16(0x4FFC); // Yes, we are using big endian
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
				cpu.setRegister(instr.rd, cpu.getRegister(instr.rt) << instr.shamt);
				break;

			case 0x02: // SRL
				cpu.setRegister(instr.rd, cpu.getRegister(instr.rt) >> instr.shamt);
				break;

			case 0x03: // SRA
				cpu.setRegister(instr.rd, static_cast<int32_t>(cpu.getRegister(instr.rt)) >> instr.shamt);
				break;

			case 0x08: // JR
				cpu.pc = cpu.getRegister(instr.rs) & 0xFFFF;
				// I don't think i'll byte align jump instructions
				break;

			case 0x09: // JALR
				cpu.setRegister(instr.rd, cpu.pc);  // Save return address
				cpu.pc = cpu.getRegister(instr.rs) & 0xFFFF;
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
				cpu.setRegister(instr.rd, (static_cast<int32_t>(cpu.getRegister(instr.rs)) < static_cast<int32_t>(cpu.getRegister(instr.rt))) ? 1 : 0);
				break;

			case 0x2b: // SLTU
				cpu.setRegister(instr.rd, (cpu.getRegister(instr.rs) < cpu.getRegister(instr.rt)) ? 1 : 0);
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
			cpu.setRegister(instr.rt, (cpu.getRegister(instr.rs) < static_cast<uint32_t>(instr.imm)) ? 1 : 0);
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
			cpu.setRegister(instr.rt, instr.imm << 16);
			break;

		case 0x20: // LB
			cpu.setRegister(instr.rt, static_cast<int8_t>(mem.read8(cpu.getRegister(instr.rs) + instr.imm)));
			break;

		case 0x21: // LH
			cpu.setRegister(instr.rt, static_cast<int16_t>(mem.read16(cpu.getRegister(instr.rs) + instr.imm)));
			break;

		case 0x23: // LW
			cpu.setRegister(instr.rt, mem.read32(cpu.getRegister(instr.rs) + instr.imm));
			break;

		case 0x24: // LBU
			cpu.setRegister(instr.rt, static_cast<uint8_t>(mem.read8(cpu.getRegister(instr.rs) + instr.imm)));
			break;

		case 0x25: // LHU
			cpu.setRegister(instr.rt, static_cast<uint16_t>(mem.read16(cpu.getRegister(instr.rs) + instr.imm)));
			break;

		case 0x28: // SB
			mem.write8(cpu.getRegister(instr.rs) + instr.imm, static_cast<uint8_t>(cpu.getRegister(instr.rt)));
			break;

		case 0x29: // SH
			mem.write16(cpu.getRegister(instr.rs) + instr.imm, static_cast<uint16_t>(cpu.getRegister(instr.rt)));
			break;

		case 0x2b: // SW
			mem.write32(cpu.getRegister(instr.rs) + instr.imm, cpu.getRegister(instr.rt));
			break;

	}

}
