.globl main

.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text
fun: # a0= (a0+1)*(-a0)
    # a0 is x!

    # t0=a0+1
    addi t0, a0, 1
    # t1 = -a0
    sub t1, x0, a0
    # a0 = (a0+1)*(-a0)
    mul a0, t0, t1
    # alias of ret
    jr ra

main:
    # BEGIN PROLOGUE
    addi sp, sp, -20
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw ra, 16(sp)
    # END PROLOGUE
    addi t0, x0, 0
    addi s0, x0, 0
    la s1, source
    la s2, dest
loop:
    slli s3, t0, 2 # s3 = k*(sizeof(int))

    add t1, s1, s3 # t1 = source+k*4
    # t1 = &(source[k])

    lw t2, 0(t1) # t2 = *t1
    # t2 = source[k]

    # if t2 == 0: jmp exit!
    beq t2, x0, exit

    # loop starts!
    # a0 = t2(source[k])
    add a0, x0, t2

    # save reg t0 and t2
    addi sp, sp, -8
    sw t0, 0(sp)
    sw t2, 4(sp)
    jal fun  # x is a0
    # load reg
    lw t0, 0(sp)
    lw t2, 4(sp)
    addi sp, sp, 8
    # a0 = fun(source[k])

    add t2, x0, a0
    add t3, s2, s3

    # *t3 = t2
    sw t2, 0(t3)
    add s0, s0, t2
    addi t0, t0, 1 # t0 is k
    jal x0, loop
exit:
    add a0, x0, s0
    # BEGIN EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    # END EPILOGUE
    jr ra
