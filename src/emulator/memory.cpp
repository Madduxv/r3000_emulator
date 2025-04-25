
#include <cctype>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
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

/**
 * @brief Reads 8 bits at the given address from the Memory object.
 *
 * @param address   The address to read from.
 * @return uint8_t  The byte at the given address.
 */
uint8_t Memory::read8(uint32_t address) {
  return mem[address & (MEM_SIZE - 1)];
}

/**
 * @brief Reads 16 bits at the given address from the Memory object.
 *
 * @param address   The address to read from.
 * @return uint8_t  The two bytes at the given address.
 */
uint16_t Memory::read16(uint32_t address) {
  return (Memory::read8(address) << 8) | Memory::read8((address + 1) & (MEM_SIZE - 1));
}

/**
 * @brief Reads 32 bits at the given address from the Memory object.
 *
 * @param address   The address to read from.
 * @return uint8_t  The word at the given address.
 */
uint32_t Memory::read32(uint32_t address) {
  return (Memory::read16(address) << 16) | Memory::read16((address + 2) & (MEM_SIZE - 1));
}

/**
 * @brief Writes 8 bits to the given address in the Memory object.
 *
 * @param address   The address to write to.
 * @param value     The data to write.
 */
void Memory::write8(uint32_t address, uint8_t value) {
  mem[address & (MEM_SIZE - 1)] = value;
}

/**
 * @brief Writes 16 bits to the given address in the Memory object.
 *
 * @param address   The address to write to.
 * @param value     The data to write.
 */
void Memory::write16(uint32_t address, uint16_t value) {
  Memory::write8(address, value >> 8);
  Memory::write8((address + 1) & (MEM_SIZE - 1), value & 0xFF);
}

/**
 * @brief Writes 32 bits to the given address in the Memory object.
 *
 * @param address   The address to write to.
 * @param value     The data to write.
 */
void Memory::write32(uint32_t address, uint32_t value) {
  Memory::write16(address, value >> 16);
  Memory::write16((address + 2) & (MEM_SIZE - 1), value & 0xFFFF);
}


void Memory::hexdump() {
  std::cout << "ROM: " << std::endl;
  printSegment32(0, 0x4FFF);

  std::cout << "\nStatic Variables: " << std::endl;
  printSegment8(0x5000, 0x6FFF);

  std::cout << "\nHeap: " << std::endl;
  printSegment32(0x7000, 0x9FFF);

  std::cout << "\nStack: " << std::endl;
  printSegment32(0xA000, 0xBFFF);

}

void Memory::printSegment32(uint32_t startAddr, uint32_t endAddr) {
  // I wanted to keep the instructions in-tact, so I won't make this look like hexdump -C
  for (int i = startAddr; i < endAddr; i+=4) {
    uint32_t val = read32(i);
    if (0 != val) {
      std::cout << "0x"
        << std::setfill('0') << std::setw(4) 
        << std::hex << std::uppercase << i 
        << " : 0x" << std::setfill('0') 
        << std::setw(8) << std::hex 
        << std::uppercase << val << std::endl;
    }
  }
}


void Memory::printSegment8(uint32_t startAddr, uint32_t endAddr) {
  const int bytesPerLine = 16;
  // Yes, I had to look up how to spell asterisk
  bool asteriskAlready = false;

  for (uint32_t i = startAddr; i < endAddr; i += bytesPerLine) {
    bool allZero = true;
    for (int j = 0; j < bytesPerLine; j++) {
      if (read8(i + j) != 0) {
        allZero = false;
        break;
      }
    }

    if (allZero) {
      if (!asteriskAlready) {
        std::cout << "*" << std::endl;
        asteriskAlready = true;
      }
      continue;
    }

    std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << i << "  ";

    for (int j = 0; j < bytesPerLine; j++) {
      uint8_t val = read8(i + j);
      std::cout << std::setw(2) << static_cast<int>(val) << " ";
      if (j == 7) std::cout << " "; 
    }

    // Dang this looks cool
    std::cout << " |";
    for (int j = 0; j < bytesPerLine; j++) {
      uint8_t val = read8(i + j);
      char c = static_cast<char>(val);
      std::cout << (std::isprint(c) ? c : '.');
    }
    std::cout << "|" << std::endl;
  }
}

/*void Memory::printSegment8(uint32_t startAddr, uint32_t endAddr) {*/
/*  for (int i = startAddr; i < endAddr; i++) {*/
/*    uint8_t val = read8(i);*/
/*    char c = static_cast<char>(val);*/
/*    if (0 != val) {*/
/*      std::cout << "0x"*/
/*        << std::setfill('0') << std::setw(4) */
/*        << std::hex << std::uppercase << i */
/*        << " : 0x" << std::setfill('0') */
/*        << std::setw(2) << std::hex */
/*        << std::uppercase << static_cast<int>(val) */
/*        << " " << (std::isprint(c) ? c : '.') << std::endl;*/
/*    }*/
/*  }*/
/*}*/

