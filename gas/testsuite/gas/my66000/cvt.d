#name: CVT
#objdump: -d

.*:     file format .*

Disassembly of section .text:

0+ <foo>:
   0:	28220828 	cvtus	r1,r2
   4:	28641025 	cvtsu	r3,r4,r5
   8:	28c70840 	cvtsd	r6,r7,#0
   c:	29093848 	cvtsf	r8,-r9
  10:	29452868 	cvtud	r10,-r5
  14:	29677868 	cvtuf	r11,#-7
  18:	298d0a24 	cvtdu	r12,r13,#4
  1c:	29cf1a28 	cvtds	r14,r15
  20:	2a110a43 	cvtfu	r16,r17,#3
  24:	2a531a44 	cvtfs	r18,r19,#4
  28:	2a952a68 	cvtdf	r20,-r21
  2c:	2ad71a68 	cvtfd	r22,r23
  30:	2b190a88 	rnd	r24,r25
  34:	2b5b3a88 	rndf	r26,-r27
