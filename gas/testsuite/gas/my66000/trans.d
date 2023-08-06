#name: Transcendental functions
#objdump: -d
.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	34010300 	fln2	r0,r1
   4:	34431300 	fln2f	r2,r3
   8:	34852301 	fln	-r4,r5
   c:	34c71301 	flnf	r6,r7
  10:	35090302 	flog	r8,r9
  14:	354b3302 	flogf	-r10,r11
  18:	358d4304 	fexp2	r12,-r13
  1c:	35cf5304 	fexp2f	r14,-r15
  20:	36116305 	fexp	-r16,-r17
  24:	36531305 	fexpf	r18,r19
  28:	36950308 	frcp	r20,r21
  2c:	36d73308 	frcpf	-r22,r23
  30:	3719030c 	frsqrt	r24,r25
  34:	375b130c 	frsqrtf	r26,r27
  38:	379d230d 	fsqrt	-r28,r29
  3c:	3401530d 	fsqrtf	r0,-r1
  40:	34434310 	fln2p1	r2,-r3
  44:	34857310 	fln2p1f	-r4,-r5
  48:	34c70311 	flnp1	r6,r7
  4c:	35091311 	flnp1f	r8,r9
  50:	354b2312 	flogp1	-r10,r11
  54:	358d1312 	flogp1f	r12,r13
  58:	35cf0314 	fexp2m1	r14,r15
  5c:	36113314 	fexp2m1f	-r16,r17
  60:	36534315 	fexpm1	r18,-r19
  64:	36955315 	fexpm1f	r20,-r21
  68:	36d76316 	fexp10m1	-r22,-r23
  6c:	37191316 	fexp10m1f	r24,r25
  70:	375b0318 	fsin	r26,r27
  74:	379d3318 	fsinf	-r28,r29
  78:	34010319 	fcos	r0,r1
  7c:	34431319 	fcosf	r2,r3
  80:	3485231a 	ftan	-r4,r5
  84:	34c7531a 	ftanf	r6,-r7
  88:	3509431c 	fasin	r8,-r9
  8c:	354b731c 	fasinf	-r10,-r11
  90:	358d031d 	facos	r12,r13
  94:	35cf131d 	facosf	r14,r15
  98:	3611231e 	fatan	-r16,r17
  9c:	3653131e 	fatanf	r18,r19
