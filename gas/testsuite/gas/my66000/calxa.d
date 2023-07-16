#name: CALX and CALA
#objdump: -d

.*:     file format .*


Disassembly of section .text:

0+ <foo>:
   0:	240084e0 	cala	\[ip,12\]
   4:	0000000c 
   8:	6be00000 	ret

000000000000000c <bar>:
   c:	24008c60 	calx	\[ip,0\]
  10:	00000000 
  14:	00000000 
  18:	6be00000 	ret
