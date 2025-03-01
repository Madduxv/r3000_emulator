#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>

struct CPU {
	uint32_t registers[32]; 
public:
	uint32_t pc = 0;
	uint32_t hi = 0, lo = 0;

	CPU();
	uint32_t getRegister(int index);
	void setRegister(int index, uint32_t value);
	void print();
};


#endif // CPU_HPP
