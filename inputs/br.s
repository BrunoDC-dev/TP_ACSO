.text

start:
 X0, foo       
BR X0             

bar:
MOV X1, 0x1     
HLT 0             

foo:
MOV X2, 0x2      
BR bar            