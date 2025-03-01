.globl _main

_main:
				li $t1, 0x2a # Ah yes this is a comment
				li $t2, 0x01
				add $t3, $t1, $t2
				li $v0, 10
				syscall
	// this is also a comment /
.end _main
