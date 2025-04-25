#include "assembler/assembler.hpp"
#include "emulator/emulator.hpp"
#include "emulator/memory.hpp"
#include "emulator/cpu.hpp"
#include <iostream>
#include <ostream>
#include <string>

int main (int argc, char *argv[]) {
  
  // J-type
  /*| OPCODE | Address | */
  /*| 6-bits | 26-bits | */

  // R-type
  /*| OPCODE |  RS   |  RT   |  RD   | SHAMT | FUNCT | */
  /*| 6-bits | 5-bit | 5-bit | 5-bit | 5-bit | 6-bit | */

  // I-type
  /*| OPCODE |  RS   |  RT   | Immediate | */
  /*| 6-bits | 5-bit | 5-bit | 16-bits   | */

  if(argc < 2) {
    std::cout << "Usage: ./r3000_emu <file>" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];

  Memory mem;
  CPU cpu;

  Assembler assembler(filename, mem);
  assembler.assemble(mem);
  /*mem.hexdump();*/
  startup(mem, cpu);
  run(mem, cpu);
  /*cpu.print();*/

  return 0;
}
