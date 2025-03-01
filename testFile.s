.globl _main

_main:
				li $t1, 0x2a
				li $t2, 0x01
				add $t3, $t1, $t2
				li $v0, 10
				syscall
.end _main
