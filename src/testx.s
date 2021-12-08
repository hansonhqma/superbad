addi t0 zero 42
jal 6
addi v0 zero 100
sub s1 v0 t0
sw s1 s0 2
j 10000
sw t0 sp 1
addi t1 zero 17
sub a0 t0 t1
lw s0 sp 1
and s1 a0 s0
jr ra
