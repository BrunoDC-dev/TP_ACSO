.text

// Paso 1: Preparar el valor a guardar
movz X1, 0xABCD      // X1 = 0x0000ABCD

// Paso 2: Preparar la base de memoria: X2 = 0x10000000
movz X2, 0x1000
lsl X2, X2, #16       // X2 = 0x10000000

// Paso 3: Guardar los 16 bits de W1 en memoria [X2 + 0x10]
sturh W1, [X2, #0x10]

// Paso 4: Borrar X1 para comprobar la carga
movz X1, 0x0          // X1 = 0

// Paso 5: Cargar desde [X2 + 0x10] → W1
ldurh W1, [X2, #0x10]

// Esperamos: W1 = 0x0000ABCD
HLT 0
