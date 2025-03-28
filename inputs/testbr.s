.text
movz x1, #0x1000
lsl x1, x1, #16
adds x1, x1, #12
br x1

// Dirección 0x1000000C
adds x2, x2, #42
HLT 0
