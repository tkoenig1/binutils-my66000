foo:
	beq0	r1,.L1
	bne0	r1,.L1
	bge0	r1,.L1
	blt0	r1,.L1
	bgt0	r1,.L1
	ble0	r1,.L1
.L1:
	bdeq	r17,.L2
	bdne	r17,.L2
	bdge	r17,.L2
	bdlt	r17,.L2
	bdgt	r17,.L2
	bdle	r17,.L2
	bdor	r17,.L2
	bdun	r17,.L2
.L2:
	bfeq	r30,.L3
	bfne	r30,.L3
	bfge	r30,.L3
	bflt	r30,.L3
	bfgt	r30,.L3
	bfle	r30,.L3
	bfor	r30,.L3
	bfun	r30,.L3
	.L3:
	ret

	
