#name: 2-operand floating point
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <.text>:
   0:	28010622 	fadd	r0,r1,r2
   4:	28644645 	fmul	r3,-r4,r5
   8:	28c72668 	fdiv	r6,r7,-r8
   c:	292a668b 	fcmp	r9,-r10,-r11
  10:	298d06ce 	fmax	r12,r13,r14
  14:	29f046f1 	fmin	r15,-r16,r17
  18:	2a532734 	csgn	r18,r19,-r20
  1c:	2ab667b7 	pow	r21,-r22,-r23
  20:	2b1907da 	atan2	r24,r25,r26
  24:	2b7c563d 	faddf	r27,-r28,r29
  28:	28013642 	fmulf	r0,r1,-r2
  2c:	28647665 	fdivf	r3,-r4,-r5
  30:	28c71688 	fcmpf	r6,r7,r8
  34:	292a56cb 	fmaxf	r9,-r10,r11
  38:	298d36ee 	fminf	r12,r13,-r14
  3c:	29f07731 	csgnf	r15,-r16,-r17
  40:	2a5317b4 	powf	r18,r19,r20
  44:	2ab657d7 	atan2f	r21,-r22,r23
  48:	2b19275a 	eadd	r24,r25,-r26
  4c:	2b7c775d 	eaddf	r27,-r28,-r29
  50:	28010e21 	fadd	r0,r1,#1
  54:	28434e42 	fmul	r2,r3,#-2
  58:	28832e65 	fdiv	r4,#3,r5
  5c:	28c46e87 	fcmp	r6,#-4,r7
  60:	29090ec5 	fmax	r8,r9,#5
  64:	294b4ee6 	fmin	r10,r11,#-6
  68:	29872f2d 	csgn	r12,#7,r13
  6c:	29c86faf 	pow	r14,#-8,r15
  70:	2a110fc9 	atan2	r16,r17,#9
  74:	2a535e2a 	faddf	r18,r19,#-10
  78:	2a8b3e55 	fmulf	r20,#11,r21
  7c:	2acc7e77 	fdivf	r22,#-12,r23
  80:	2b191e8d 	fcmpf	r24,r25,#13
  84:	2b5b5ece 	fmaxf	r26,r27,#-14
  88:	2b8f3efd 	fminf	r28,#15,r29
  8c:	28107f21 	csgnf	r0,#-16,r1
  90:	28431fb1 	powf	r2,r3,#17
  94:	28855fd2 	atan2f	r4,r5,#-18
  98:	28d32f47 	eadd	r6,#19,r7
  9c:	29147f49 	eaddf	r8,#-20,r9
  a0:	294b8620 	fadd	r10,r11,#0xd01502f9
  a4:	d01502f9 
  a8:	2980a64d 	fmul	r12,#0xbfc00000,r13
  ac:	bfc00000 
  b0:	29cfc660 	fdiv	r14,-r15,#0x3eaaaaab
  b4:	3eaaaaab 
  b8:	2a00e691 	fcmp	r16,#0x3fc90fdb,-r17
  bc:	3fc90fdb 
  c0:	2a5386c0 	fmax	r18,r19,#0xd01502f9
  c4:	d01502f9 
  c8:	2a80a6f5 	fmin	r20,#0xbfc00000,r21
  cc:	bfc00000 
  d0:	2ad7c720 	csgn	r22,-r23,#0x3eaaaaab
  d4:	3eaaaaab 
  d8:	2b00e7b9 	pow	r24,#0x3fc90fdb,-r25
  dc:	3fc90fdb 
  e0:	2b5b87c0 	atan2	r26,r27,#0xd01502f9
  e4:	d01502f9 
  e8:	2b80b63d 	faddf	r28,#0xbfc00000,r29
  ec:	bfc00000 
  f0:	2801d640 	fmulf	r0,-r1,#0x3eaaaaab
  f4:	3eaaaaab 
  f8:	2840f663 	fdivf	r2,#0x3fc90fdb,-r3
  fc:	3fc90fdb 
 100:	28859680 	fcmpf	r4,r5,#0xd01502f9
 104:	d01502f9 
 108:	28c0b6c7 	fmaxf	r6,#0xbfc00000,r7
 10c:	bfc00000 
 110:	2909d6e0 	fminf	r8,-r9,#0x3eaaaaab
 114:	3eaaaaab 
 118:	2940f72b 	csgnf	r10,#0x3fc90fdb,-r11
 11c:	3fc90fdb 
 120:	298d97a0 	powf	r12,r13,#0xd01502f9
 124:	d01502f9 
 128:	29c0b7cf 	atan2f	r14,#0xbfc00000,r15
 12c:	bfc00000 
 130:	2a11ce20 	fadd	r16,-r17,#0x3fd5555555555555
 134:	55555555 
 138:	3fd55555 
 13c:	2a40ee53 	fmul	r18,#0x3ff921fb54442d18,-r19
 140:	54442d18 
 144:	3ff921fb 
 148:	2a958e60 	fdiv	r20,r21,#0xc202a05f20000000
 14c:	20000000 
 150:	c202a05f 
 154:	2ac0ae97 	fcmp	r22,#0xbff8000000000000,r23
 158:	00000000 
 15c:	bff80000 
 160:	2b19cec0 	fmax	r24,-r25,#0x3fd5555555555555
 164:	55555555 
 168:	3fd55555 
 16c:	2b40eefb 	fmin	r26,#0x3ff921fb54442d18,-r27
 170:	54442d18 
 174:	3ff921fb 
 178:	2b9d8f20 	csgn	r28,r29,#0xc202a05f20000000
 17c:	20000000 
 180:	c202a05f 
 184:	2800afa1 	pow	r0,#0xbff8000000000000,r1
 188:	00000000 
 18c:	bff80000 
 190:	2843cfc0 	atan2	r2,-r3,#0x3fd5555555555555
 194:	55555555 
 198:	3fd55555 
 19c:	2880a745 	eadd	r4,#0xbfc00000,r5
 1a0:	bfc00000 
 1a4:	28c0f747 	eaddf	r6,#0x3fc90fdb,-r7
 1a8:	3fc90fdb 
 1ac:	2900af49 	eadd	r8,#0x3fd5555555555555,r9
 1b0:	55555555 
 1b4:	3fd55555 
 1b8:	2940f74b 	eaddf	r10,#0x3fc90fdb,-r11
 1bc:	3fc90fdb 
 1c0:	298d8740 	eadd	r12,r13,#512
 1c4:	00000200 
 1c8:	29cfd740 	eaddf	r14,-r15,#-32
 1cc:	ffffffe0 
