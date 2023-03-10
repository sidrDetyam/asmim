.section .text

.globl foo

foo:

.loop:
    movq %rsi, %rcx
    imulq $64, %rcx
    addq %rdi, %rcx
    movq (%rcx), %rsi
