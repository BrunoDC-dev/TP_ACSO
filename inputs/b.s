.text

mov X0, 0x1       
cmp X0, 0x1       
beq label_equal   
b label_end   


label_equal:
mov X1, 0x2      
b label_end

label_end:
HLT 0