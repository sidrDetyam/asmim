    subq $1, %rdx
    testq %rdx, %rdx
    jnz .loop

    ret
