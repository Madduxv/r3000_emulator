#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "cpu.hpp"
#include "instructions.hpp"
#include "memory.hpp"

void startup(Memory& mem, CPU &cpu);
void execute(Instruction &instr, CPU &cpu, Memory &mem);
void run(Memory &mem, CPU &cpu);

#endif // !EMULATOR_HPP
