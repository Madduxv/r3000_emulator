.globl _main

.data

  hello: .ascii "Hello, World!\n\0"
  aWord: .word 42069
  array: .space 20
  hello2: .asciiz "Asm is easy\n"


.text

_main:
  addi $t1, $zero, 0x2a # Ah yes this is a comment
  addi $t2, $zero, 0x01
  add $t3, $t1, $t2

  #addi $v0, $zero, 4
  li $v0, 4
  nop
  nop
  la $a1, hello
  li $t3, 4
  addi $t4, $zero, 1
loop: 
  syscall
  sub $t3, $t3, $t4
  bgtz $t3, loop

  ori $v0, $zero, 0
  li $t0, 4
  move $v0, $t0
  la $a1, hello2
  la $a1, hello2
  syscall

  addi $v0, $zero, 10
  syscall
  // this is also a comment /
  li $v0, 1
  lw $a1, aWord
  syscall

.end _main


