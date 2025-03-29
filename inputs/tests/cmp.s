
cmp x0,0 
beq foo
adds x0 ,x0 , 5
hlt 0


foo2:
adds x2, x2, 8
hlt 0

foo:
cmp x0 ,x0
beq foo2
adds x1,x1,7
hlt 0
