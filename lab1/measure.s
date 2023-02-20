.section .text

.globl measure_latency
measure_latency:
    movl %edi, %ecx
    imull $6, %edi
    pxor %xmm0, %xmm0
    pxor %xmm1, %xmm1
    cvtsi2sdl %edi, %xmm1

    lfence
    rdtsc
    shlq $32, %rdx
    orq %rdx, %rax
    movq %rax, %rdi

    lfence
.loop:
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2
    mulsd %xmm2, %xmm2

    subl $1, %ecx
    jnz .loop

    lfence
    rdtsc
    shlq $32, %rdx
    orq %rdx, %rax
    subq %rdi, %rax
    cvtsi2sdq %rax, %xmm0

    divsd %xmm1, %xmm0
    ret
