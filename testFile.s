.globl _main

.data

  hello: .ascii "Hello, World!\n"
  aWord: .word 42069
  array: .space 20
  hello2: .asciiz "Asm is easy\n"


.text

_main:
  addi $t1, $zero, 0x2a # Ah yes this is a comment
  addi $t2, $zero, 0x01
  add $t3, $t1, $t2

  addi $v0, $zero, 4
  ori $a1, hello
  addi $a2, $zero, 14
  #addi $t3, $zero, 4
  #addi $t4, $zero, 1
  syscall
  #sub $t3, $t3, $t4
  #bgtz, $t3, -3

  addi $v0, $zero, 4
  ori $a1, hello2
  addi $a2, $zero, 14
  syscall

  addi $v0, $zero, 10
  syscall
  j 2
  // this is also a comment /
.end _main


