.text
adds X11, X3, 0x1
adds X10, X3, 0x10
LDR X1, [ X2, LSL #3, #16]
eor X0, X10, X11, LSL #2
eor X1, X11, X12
eor X2, X12, X13

HLT 0
