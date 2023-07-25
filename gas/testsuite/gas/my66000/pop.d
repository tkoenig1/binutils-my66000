#name: Bit manipulation
#objdump: -d

.*:     file format .*

Disassembly of section .text:

0+ <foo>:
   0:	34220100 	pop	r1,r2
   4:	34644100 	pop	r3,-r4
   8:	34a60120 	cttz	r5,r6
   c:	34a64120 	cttz	r5,-r6
  10:	34e81120 	ctlz	r7,r8
  14:	34e85120 	ctlz	r7,-r8
