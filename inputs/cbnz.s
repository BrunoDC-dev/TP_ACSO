.text

start:
    MOV X0, 1          // Load 1 into X0
    CBNZ X0, label_not_zero // If X0 is not zero, branch to label_not_zero

    MOV X1, 0          // If X0 is zero, this instruction will execute
    B end               // Branch to the end

label_not_zero:
    MOV X1, 2          // If X0 is not zero, this instruction will execute

end:
    HLT  0
    
HLT 0               // Stop execution

