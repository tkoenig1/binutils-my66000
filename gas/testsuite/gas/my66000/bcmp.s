foo:
	.L1:
	cmp	r1,r2,#5
	beq	r1,.L1
	bneq	r1,.L2
	bb1	0,r1,.L1
	bb1	1,r1,.L2
	bne	r1,.L1
	bnne	r1,.L2
	bge	r1,.L1
	bnge	r1,.L2
	blt	r1,.L1
	bnlt	r1,.L2
	bgt	r1,.L1
	bngt	r1,.L2
	ble	r1,.L1
	bnle	r1,.L2
	bhs	r1,.L1
	blo	r1,.L2
	bhi	r1,.L1
	bls	r1,.L2
	bor	r1,.L1
	bnor	r1,.L2
	bto	r1,.L1
	bnto	r1,.L2
	bsnan	r1,.L1
	bqnan	r1,.L2
	bminf	r1,.L1
	bmnor	r1,.L2
	bmde	r1,.L1
	bmze	r1,.L2
	bpze	r1,.L1
	bpde	r1,.L2
	bpnor	r1,.L1
	bpinf	r1,.L2
	bb1	62,r1,.L1
	bb1	63,r1,.L2
	.L2:
	nop
