#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>
#include <cstdint>

	/* Memory regions:
 * 0x0000 - 0x4FFF - ROM/Program
 * 0x4FFC - 0x4FFD - Reset Vector 
 * 0x5000 - 0x6FFF - Variables
 * 0x7000 - 0x9FFF - Heap
 * 0xA000 - 0xBFFF - Stack
 * 0xE000 - 0xEFFF - Peripherals (stdout or whatever I add)
 * 0xF000 - 0xFFFF - Unused for now
 */

class Memory {
public:
    static const uint32_t MEM_SIZE = 0x10000;
    Memory();
    uint8_t read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);

private:
    std::vector<uint8_t> mem;
};

#endif
