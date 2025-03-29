.text

start:
    ADD X0, X0, #5       // Suma 5 al valor de X0
    ADD X1, X1, #2       // Suma 2 al valor de X1
    ADD X2, X1, X0       // Suma los valores de X1 y X0, almacena el resultado en X2
    HLT  0                // Detiene la ejecución