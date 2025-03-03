
#include <cstdint>
#include <vector>
#include "emulator/memory.hpp"


	/* Memory regions:
 * 0x0000 - 0x4FFF - ROM/Program
 * 0x4FFC - 0x4FFD - Reset Vector 
 * 0x5000 - 0x6FFF - Variables
 * 0x7000 - 0x9FFF - Heap
 * 0xA000 - 0xBFFF - Stack
 * 0xE000 - 0xEFFF - Peripherals (stdout or whatever I add)
 * 0xF000 - 0xFFFF - Unused for now
 */

	static const uint32_t MEM_SIZE = 0x10000; // Yes only 64kb
	std::vector<uint8_t> mem;

	Memory::Memory() : mem(MEM_SIZE, 0) {}

	uint8_t Memory::read8(uint32_t address) {
		return mem[address & (MEM_SIZE - 1)];
	}

	uint16_t Memory::read16(uint32_t address) {
		return (Memory::read8(address) << 8) | Memory::read8((address + 1) & (MEM_SIZE - 1));
	}

	uint32_t Memory::read32(uint32_t address) {
		return (Memory::read16(address) << 16) | Memory::read16((address + 2) & (MEM_SIZE - 1));
	}

	void Memory::write8(uint32_t address, uint8_t value) {
		mem[address & (MEM_SIZE - 1)] = value;
	}

	void Memory::write16(uint32_t address, uint16_t value) {
		Memory::write8(address, value >> 8);
		Memory::write8((address + 1) & (MEM_SIZE - 1), value & 0xFF);
	}

	void Memory::write32(uint32_t address, uint32_t value) {
		Memory::write16(address, value >> 16);
		Memory::write16((address + 2) & (MEM_SIZE - 1), value & 0xFFFF);
	}

