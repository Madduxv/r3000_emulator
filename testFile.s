.globl _main

_main:
				addi $t1, $zero, 0x2a # Ah yes this is a comment
				addi $t2, $zero, 0x01
				add $t3, $t1, $t2
				addi $v0, $zero, 10
				syscall
	// this is also a comment /
.end _main
