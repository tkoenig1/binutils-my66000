foo:
	pb1	1,r3,2,3
	add	r1,r2,r3
	add	r2,r4,#3
	pb1	63,r3,TTF
	add	r3,r2,r1
	add	r1,r2,r3
	add	r5,r3,r4
	peq0	r3,TTFF
	add	r3,r5,#2
	pne0	r5,1,0
	pfun	r17,TTFFF
	add	r1,r2,r3
	add	r1,r4,r2
