
.text
start:
    MOVZ X1, 0x4000   
    lsl x1,x1, 8       // Carga un valor base en X1 (simula una dirección base)
    adds x1,x1 , 0x14
    BR X1                     // Salta a la dirección almacenada en X2
    adds x2, x3,16
    adds x3 ,x4,32
    
    HLT 0
