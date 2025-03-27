
.text
adds X0, X10, 1
adds X1, X11, 11
adds X2, X12, 12
cmp X11, X12
cmp X1, X0
b foo
adds X7, X1, 25

foo2:
adds x1, x20, 4
lsl x1, x1, 4
adds x1, x20, 4
br x1

hlt:
HLT 0

foo:
subs x2, x2, x1    
subs x2, x2, x0
b foo2


HLT 0
