.text

// X1 = dato a guardar (16 bits)
movz X1, 0xABCD      // W1 = 0xABCD

// Construir X2 = 0x10000000 usando sumas
movz X2, 0x1000      // X2 = 0x00000000_00001000
lsl X2, X2, #16      // X2 = X2 << 16 → 0x10000000

// Guardar 16 bits desde W1 en [X2 + 0x10]
sturh W1, [X2, #0x10]

HLT 0
