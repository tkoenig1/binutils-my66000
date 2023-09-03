myfmac:
	fmac	r1,r2,r3,r4
	fmac	r1,r2,r3,-r4
	fmac	r1,r2,-r3,r4
	fmac	r1,r2,-r3,-r4
	fmac	r1,r2,r3,#0x12345678
	fmac	r1,r2,#0xdeadbeef,r4
	fmac	r1,r2,#0xdeadbeefdeadbeef,r4
	fmac	r1,r2,r3,#0x98765432deadbeef
	fmacf	r1,r2,r3,r4
	fmacf	r1,r2,r3,-r4
	fmacf	r1,r2,-r3,r4
	fmacf	r1,r2,-r3,-r4
	fmacf	r1,r2,r3,#0x12345678
	fmacf	r1,r2,#0xdeadbeef,r4
