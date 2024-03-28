#name: Predicate instructions
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	182300c2 	pneq	r3,TTFFF
   4:	28220423 	add	r1,r2,r3
   8:	c4440003 	add	r2,r4,#3
   c:	1fe30042 	pb1	63,r3,TTF
  10:	28620421 	add	r3,r2,r1
  14:	28220423 	add	r1,r2,r3
  18:	28a30424 	add	r5,r3,r4
  1c:	18031082 	peq0	r3,TTFF
  20:	c4650002 	add	r3,r5,#2
  24:	18251003 	pne0	r5,TTT
  28:	1af110c2 	pfun	r17,TTFFF
  2c:	28220423 	add	r1,r2,r3
  30:	28240422 	add	r1,r4,r2
