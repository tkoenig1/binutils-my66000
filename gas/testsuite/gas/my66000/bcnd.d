#name: BCND
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	68010006 	beq0	r1,18 <foo\+0x18>
   4:	68210005 	bne0	r1,18 <foo\+0x18>
   8:	68410004 	bge0	r1,18 <foo\+0x18>
   c:	68610003 	blt0	r1,18 <foo\+0x18>
  10:	68810002 	bgt0	r1,18 <foo\+0x18>
  14:	68a10001 	ble0	r1,18 <foo\+0x18>
  18:	69110008 	bdeq	r17,38 <foo\+0x38>
  1c:	69310007 	bdne	r17,38 <foo\+0x38>
  20:	69510006 	bdge	r17,38 <foo\+0x38>
  24:	69710005 	bdlt	r17,38 <foo\+0x38>
  28:	69910004 	bdgt	r17,38 <foo\+0x38>
  2c:	69b10003 	bdle	r17,38 <foo\+0x38>
  30:	69d10002 	bdor	r17,38 <foo\+0x38>
  34:	69f10001 	bdun	r17,38 <foo\+0x38>
  38:	6a1e0008 	bfeq	r30,58 <foo\+0x58>
  3c:	6a3e0007 	bfne	r30,58 <foo\+0x58>
  40:	6a5e0006 	bfge	r30,58 <foo\+0x58>
  44:	6a7e0005 	bflt	r30,58 <foo\+0x58>
  48:	6a9e0004 	bfgt	r30,58 <foo\+0x58>
  4c:	6abe0003 	bfle	r30,58 <foo\+0x58>
  50:	6ade0002 	bfor	r30,58 <foo\+0x58>
  54:	6afe0001 	bfun	r30,58 <foo\+0x58>
  58:	6be00000 	ret
