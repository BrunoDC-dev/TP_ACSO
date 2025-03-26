.text
    // Inicialización de registros con valores de ejemplo
    MOV X10, #0x0F      // X10 = 0x0F (00001111 en binario)
    MOV X11, #0xF0      // X11 = 0xF0 (11110000 en binario)
    MOV X12, #0x33      // X12 = 0x33 (00110011 en binario)
    MOV X13, #0x55      // X13 = 0x55 (01010101 en binario)

    // Realizar operaciones ORR entre registros
    ORR X0, X10, X11    // X0 = X10 OR X11 -> 0x0F OR 0xF0 = 0xFF (11111111 en binario)
    ORR X1, X11, X12    // X1 = X11 OR X12 -> 0xF0 OR 0x33 = 0xF3 (11110011 en binario)
    ORR X2, X12, X13    // X2 = X12 OR X13 -> 0x33 OR 0x55 = 0x77 (01110111 en binario)

    HLT 0               // Detener la ejecución
