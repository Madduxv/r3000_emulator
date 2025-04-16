# R3000 Emulator and Assembler

### Usage
To compile, run:
```
make
```
Then, you can run it with: 
```
./r3000_emu
```

### Current memory locations:
- 0x0000 - 0x4FFF - ROM/Program
- 0x4FFC - 0x4FFD - Reset Vector 
- 0x5000 - 0x6FFF - Variables
- 0x7000 - 0x9FFF - Heap
- 0xA000 - 0xBFFF - Stack
- 0xE000 - 0xEFFF - Peripherals (stdout or whatever I add)
- 0xF000 - 0xFFFF - Unused for now
 
### Currently implemented syscalls
1. print_int
2. print_float
3. print_double
4. print_string
10. exit

### Build Your Own: Sources
- https://student.cs.uwaterloo.ca/~isg/res/mips/opcodes
- https://www3.ntu.edu.sg/home/smitha/fyp_gerald/jrInstruction.html
- http://www.netyaroze-europe.com/~robertj/htmls/r3000_instructionset.html
- https://student.cs.uwaterloo.ca/~isg/res/mips/traps
- http://homepage.divms.uiowa.edu/~ghosh/1-28-10.pdf
- https://youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs&si=0SeoRsojt6gcmgl2
