#include <cstdint>
#include <map>
#include <string>


std::map<std::string, uint16_t> r_type = {
				{"syscall", 0x0c},
				{ "sll", 0x00  },
				{ "srl", 0x02  },
				{ "sra", 0x03  },
				{ "jr", 0x08  },
				{ "jalr", 0x09  },
				{ "mult", 0x18  },
				{ "multu", 0x19  },
				{ "div", 0x1a  },
				{ "divu", 0x1b  },
				{ "add", 0x20  },
				{ "addu", 0x21  },
				{ "sub", 0x22  },
				{ "subu", 0x23  },
				{ "and", 0x24  },
				{ "or", 0x25  },
				{ "xor", 0x26  },
				{ "nor", 0x27  },
				{ "slt", 0x2a  },
				{ "sltu", 0x2b  },
};

std::map<std::string, uint16_t> j_type = {
				{ "J", 0x02  },
				{ "JAL", 0x03  },
};

std::map<std::string, uint16_t> i_type = {
				{ "BEQ", 0x04  },
				{ "BNE", 0x05  },
				{ "BLEZ", 0x06  },
				{ "BGTZ", 0x07  },
				{ "ADDI", 0x08  },
				{ "ADDIU", 0x09  },
				{ "SLTI", 0x0A  },
				{ "SLTUI", 0x0B  },
				{ "ORI", 0x0D  },
				{ "XORI", 0x0E  },
				{ "LUI", 0x0F  },
				{ "LH", 0x21  },
				{ "LBU", 0x24  },
				{ "SB", 0x28  },
				{ "SW", 0x2b  },
};

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

std::map<std::string, uint16_t> registers = {
				{"zero", 0},
				{"at", 1},
				{"v0", 2},
				{"v1", 3},
				{"a0", 4},
				{"a1", 5},
				{"a2", 6},
				{"a3", 7},
				{"t0", 8},
				{"t1", 9},
				{"t2", 10},
				{"t3", 11},
				{"t4", 12},
				{"t5", 13},
				{"t6", 14},
				{"t7", 15},
				{"s0", 16},
				{"s1", 17},
				{"s2", 18},
				{"s3", 19},
				{"s4", 20},
				{"s5", 21},
				{"s6", 22},
				{"s7", 23},
				{"t8", 24},
				{"t9", 25},
				{"k0", 26},
				{"k1", 27},
				{"gp", 28},
				{"sp", 29},
				{"fp", 30},
				{"ra", 31},
};
