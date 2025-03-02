#include "assembler/parser.hpp"
#include "emulator/memory.hpp"
#include "emulator/cpu.hpp"
#include "emulator/emulator.hpp"
#include "assembler/lexer.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>

std::string readFile(const std::string& filename) {
    std::ifstream myfile(filename);
    if (!myfile.is_open()) {
        std::cout << "Unable to open file\n";
        return "";
    }

    std::string file;
    std::string line;
    while (getline(myfile, line)) {
        file.append(line + '\n');  // Preserve line breaks
    }
    myfile.close();
    return file;
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

	std::string file = readFile("testFile.s");
	std::cout << file << std::endl;

	std::vector<Token> tokenizedFile = tokenize(file);

	for (const Token& token : tokenizedFile) {
		std::cout << token << '\n';
	}
  std::cout << std::endl;

  std::vector<ASTNode> AST = parse(tokenizedFile);

	for (const ASTNode& node : AST) {
		std::cout << node << '\n';
	}
	/*cpu.print();*/

	return 0;
}
