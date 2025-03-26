.text
// X30 arranca en 0, así que X1 = 0 + 12 = 0xC
adds X1, X30, 12

br X1

// Instrucción que se debe saltear
movz X2, 0xBAD

// Esto está en la dirección 0xC
movz X2, 0x123

HLT 0
