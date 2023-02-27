.section .text

.globl measure_latency
.globl measure_throughput

measure_latency:
    movl %edi, %ecx
    imull $6, %edi
    pxor %xmm0, %xmm0
    pxor %xmm1, %xmm1
    cvtsi2sdl %edi, %xmm1

    lfence
    movq $0, %rax
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
    movq $0, %rax
    rdtsc
    shlq $32, %rdx
    orq %rdx, %rax
    subq %rdi, %rax
    cvtsi2sdq %rax, %xmm0

    divsd %xmm1, %xmm0
    ret


measure_throughput:
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
.loop1:
    mulsd %xmm2, %xmm2
    mulsd %xmm3, %xmm3
    mulsd %xmm4, %xmm4
    mulsd %xmm5, %xmm5
    mulsd %xmm6, %xmm6
    mulsd %xmm7, %xmm7

    subl $1, %ecx
    jnz .loop1

    lfence
    rdtsc
    shlq $32, %rdx
    orq %rdx, %rax
    subq %rdi, %rax
    cvtsi2sdq %rax, %xmm0

    divsd %xmm1, %xmm0
    ret
