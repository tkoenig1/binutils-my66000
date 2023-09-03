#name: FMAC
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <myfmac>:
   0:	30220083 	fmac	r1,r2,r3,r4
   4:	30222083 	fmac	r1,r2,r3,-r4
   8:	30224083 	fmac	r1,r2,-r3,r4
   c:	30226083 	fmac	r1,r2,-r3,-r4
  10:	30228003 	fmac	r1,r2,r3,#0x12345678
  14:	12345678 
  18:	3022a080 	fmac	r1,r2,#0xdeadbeef,r4
  1c:	deadbeef 
  20:	3022e080 	fmac	r1,r2,#0xdeadbeefdeadbeef,r4
  24:	deadbeef 
  28:	deadbeef 
  2c:	3022c003 	fmac	r1,r2,r3,#0x98765432deadbeef
  30:	deadbeef 
  34:	98765432 
  38:	30221083 	fmacf	r1,r2,r3,r4
  3c:	30223083 	fmacf	r1,r2,r3,-r4
  40:	30225083 	fmacf	r1,r2,-r3,r4
  44:	30227083 	fmacf	r1,r2,-r3,-r4
  48:	30229003 	fmacf	r1,r2,r3,#0x12345678
  4c:	12345678 
  50:	3022b080 	fmacf	r1,r2,#0xdeadbeef,r4
  54:	deadbeef 
