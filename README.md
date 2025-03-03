# R3000 Emulator and Assembler

### Current memory locations:
- 0x0000 - 0x4FFF - ROM/Program
- 0x4FFC - 0x4FFD - Reset Vector 
- 0x5000 - 0x6FFF - Variables
- 0x7000 - 0x9FFF - Heap
- 0xA000 - 0xBFFF - Stack
- 0xE000 - 0xEFFF - Peripherals (stdout or whatever I add)
- 0xF000 - 0xFFFF - Unused for now
 
### Currently implemented syscalls
- v0 = 1:  print_int
- v0 = 2:  print_float
- v0 = 3:  print_double
- v0 = 4:  print_string
- v0 = 10: exit
