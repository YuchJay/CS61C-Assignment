.globl factorial

.data
n: .word 8

.text
main:
    # t0 = &n
    la t0, n
    # a0 = 8
    lw a0, 0(t0)
    jal ra, factorial # pass a0 into func

    # a1 = a0
    addi a1, a0, 0
    # a0 = 1
    addi a0, x0, 1
    ecall # Print Result

    # a1 = '\n'
    addi a1, x0, '\n'
    # a0 = 11
    addi a0, x0, 11
    ecall # Print newline

    # a0 = 10
    addi a0, x0, 10
    ecall # Exit

factorial:
    # demo: if(x==1) return 1
    #       else return x*func(x-1)
    addi t0, x0, 1
    ble a0, t0, base

    addi sp, sp, -16
    sw ra, 12(sp)
    sw a0, 8(sp)

    addi a0, a0, -1
    jal ra, factorial

    lw t0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16

    mul a0, t0, a0 # func(n) = n * func(n-1)
    ret

base:
    li a0, 1
    ret