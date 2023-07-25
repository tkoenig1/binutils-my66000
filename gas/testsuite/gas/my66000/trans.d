#name: Transcendental functions
#objdump: -d
.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	34010300 	ln2	r0,r1
   4:	34431300 	ln2f	r2,r3
   8:	34852301 	ln	-r4,r5
   c:	34c71301 	lnf	r6,r7
  10:	35090302 	log	r8,r9
  14:	354b3302 	logf	-r10,r11
  18:	358d4304 	exp2	r12,-r13
  1c:	35cf5304 	exp2f	r14,-r15
  20:	36116305 	exp	-r16,-r17
  24:	36531305 	expf	r18,r19
  28:	36950308 	rcp	r20,r21
  2c:	36d73308 	rcpf	-r22,r23
  30:	3719030c 	rsqrt	r24,r25
  34:	375b130c 	rsqrtf	r26,r27
  38:	379d230d 	sqrt	-r28,r29
  3c:	3401530d 	sqrtf	r0,-r1
  40:	34434310 	ln2p1	r2,-r3
  44:	34857310 	ln2p1f	-r4,-r5
  48:	34c70311 	lnp1	r6,r7
  4c:	35091311 	lnp1f	r8,r9
  50:	354b2312 	logp1	-r10,r11
  54:	358d1312 	logp1f	r12,r13
  58:	35cf0314 	exp2m1	r14,r15
  5c:	36113314 	exp2m1f	-r16,r17
  60:	36534315 	expm1	r18,-r19
  64:	36955315 	expm1f	r20,-r21
  68:	36d76316 	exp10m1	-r22,-r23
  6c:	37191316 	exp10m1f	r24,r25
  70:	375b0318 	sin	r26,r27
  74:	379d3318 	sinf	-r28,r29
  78:	34010319 	cos	r0,r1
  7c:	34431319 	cosf	r2,r3
  80:	3485231a 	tan	-r4,r5
  84:	34c7531a 	tanf	r6,-r7
  88:	3509431c 	asin	r8,-r9
  8c:	354b731c 	asinf	-r10,-r11
  90:	358d031d 	acos	r12,r13
  94:	35cf131d 	acosf	r14,r15
  98:	3611231e 	atan	-r16,r17
  9c:	3653131e 	atanf	r18,r19
