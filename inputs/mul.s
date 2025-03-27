.text

MOV X0, 5          // Load 5 into X0
MOV X1, 3          // Load 3 into X1
MUL X2, X0, X1      // Multiply X0 and X1, store the result in X2

MOV X3, 10         // Load 10 into X3
MOV X4, 0          // Load 0 into X4
MUL X5, X3, X4      // Multiply X3 and X4, store the result in X5 (should be 0)

HLT 0