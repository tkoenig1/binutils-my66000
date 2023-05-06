#name: bcmp-class branches
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	d0220005 	cmp	r1,r2,#5
   4:	6001ffff 	beq	r1,0 \<foo\>
   8:	60210021 	bneq	r1,8c \<foo\+0x8c\>
   c:	6001fffd 	beq	r1,0 \<foo\>
  10:	6021001f 	bneq	r1,8c \<foo\+0x8c\>
  14:	6041fffb 	bne	r1,0 \<foo\>
  18:	6061001d 	bnne	r1,8c \<foo\+0x8c\>
  1c:	6081fff9 	bge	r1,0 \<foo\>
  20:	60a1001b 	bnge	r1,8c \<foo\+0x8c\>
  24:	60c1fff7 	blt	r1,0 \<foo\>
  28:	60e10019 	bnlt	r1,8c \<foo\+0x8c\>
  2c:	6101fff5 	bgt	r1,0 \<foo\>
  30:	61210017 	bngt	r1,8c \<foo\+0x8c\>
  34:	6141fff3 	ble	r1,0 \<foo\>
  38:	61610015 	bnle	r1,8c \<foo\+0x8c\>
  3c:	6181fff1 	bhs	r1,0 \<foo\>
  40:	61a10013 	blo	r1,8c \<foo\+0x8c\>
  44:	61c1ffef 	bhi	r1,0 \<foo\>
  48:	61e10011 	bls	r1,8c \<foo\+0x8c\>
  4c:	6201ffed 	bor	r1,0 \<foo\>
  50:	6221000f 	bnor	r1,8c \<foo\+0x8c\>
  54:	6241ffeb 	bto	r1,0 \<foo\>
  58:	6261000d 	bnto	r1,8c \<foo\+0x8c\>
  5c:	6401ffe9 	bsnan	r1,0 \<foo\>
  60:	6421000b 	bqnan	r1,8c \<foo\+0x8c\>
  64:	6441ffe7 	bminf	r1,0 \<foo\>
  68:	64610009 	bmnor	r1,8c \<foo\+0x8c\>
  6c:	6481ffe5 	bmde	r1,0 \<foo\>
  70:	64a10007 	bmze	r1,8c \<foo\+0x8c\>
  74:	64c1ffe3 	bpze	r1,0 \<foo\>
  78:	64e10005 	bpde	r1,8c \<foo\+0x8c\>
  7c:	6501ffe1 	bpnor	r1,0 \<foo\>
  80:	65210003 	bpinf	r1,8c \<foo\+0x8c\>
  84:	67c1ffdf 	bb1	62,r1,0 \<foo\>
  88:	67e10001 	bb1	63,r1,8c \<foo\+0x8c\>
  8c:	e0000000 	nop
