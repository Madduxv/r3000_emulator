#include "emulator/emulator.hpp"
#include "emulator/memory.hpp"
#include "emulator/cpu.hpp"
#include "emulator/instructions.hpp"
#include <cstring>
#include <ios>
#include <iostream>
#include <ostream>

/**
 * @brief Gets the starting address (in big endian) from the address 0x4FFC  
 *        and sets the program counter to that address.
 *
 * @param mem The memory object containing the machine code.
 * @param cpu A reference to the CPU object
 */
void startup(Memory& mem, CPU &cpu) {
  // Our reset vector will be at 4FFC for now
  cpu.pc = 0x00000000 + mem.read16(0x4FFC); // Yes, we are using big endian
}

/**
 * @brief Runs a machine-code program stored in the "memory" of the emulator.
 *
 * @param mem A reference to the memory object containing the machine code.
 * @param cpu A reference to the CPU object
 */
void run(Memory &mem, CPU &cpu) {
  Instruction instr; 

  do {
    instr = Instruction::decode(mem.read32(cpu.pc));
    /*std::cout << "Pc: 0x" << std::hex << cpu.pc << " Instr: 0x" << std::hex << mem.read32(cpu.pc) << std::endl;*/
    /*instr.print();*/
    execute(instr, cpu, mem);
  } while (!(mem.read32(cpu.pc) == 0x0000000C && cpu.getRegister(2) == 0x0A));
}

/**
 * @brief Executes a given machine-code instruction
 *
 * @param instr The instruction to be executed.
 * @param cpu   A reference to the CPU object
 * @param mem   A reference to the memory object containing the machine code.
 */
void execute(Instruction &instr, CPU &cpu, Memory &mem) {
// I really should have just used the 6502 instruction set
  cpu.pc = ( cpu.pc + 4 ) & 0xFFFF;

  // R-type
  if (instr.opcode == 0x00) {
    switch (instr.funct) {

      case 0x0C: { // SYSCALL
        uint32_t syscall_type = cpu.getRegister(2); 

        switch (syscall_type) {
          case 1: // print_int
            std::cout << int32_t(cpu.getRegister(4)); 
            break;

          case 2: { // print_float
            float float_val;
            uint32_t float_bits = cpu.getRegister(4);
            std::memcpy(&float_val, &float_bits, sizeof(float));
            std::cout << float_val;
            break;
          }

          case 3: { // print_double
            double double_val;
            // yes, I used | instead of + because it looks cooler :)
            uint64_t double_bits = (uint64_t(cpu.getRegister(4)) << 32) | cpu.getRegister(5);
            std::memcpy(&double_val, &double_bits, sizeof(double));
            std::cout << double_val;
            break;
          } 

          case 4: { // print_string
            int i = 0;
            char c;
            uint32_t addr = cpu.getRegister(5);
            while ((c = char(mem.read8(addr + i))) != '\0') {
              std::cout << c;
              i++;
            }
            break;
          }

          case 5: // read_int
            int inputVal;
            std::cin >> inputVal;
            cpu.setRegister(2, inputVal);
            break;

          case 10: // Exit
            std::exit(0);
            break;

          default:
            std::cerr << "Unknown syscall: " << syscall_type << std::endl;
            std::exit(1);
        }
        break;
      }

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
