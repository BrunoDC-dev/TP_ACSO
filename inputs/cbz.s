.text

start:
    MOV X0, #0          // Load 0 into X0
    CBZ X0, label_zero  // If X0 is zero, branch to label_zero

    MOV X1, #1          // If X0 is not zero, this instruction will execute
    B end               // Branch to the end

label_zero:
    MOV X1, #2          // If X0 is zero, this instruction will execute

end:
    HLT 0                // Stop execution

HLT 0