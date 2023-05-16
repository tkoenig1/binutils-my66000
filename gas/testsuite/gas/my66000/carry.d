#name: Carry instruction
#objdump:  -d

.*:     file format .*

Disassembly of section .text:

0+ <foo>:
   0:	f201007e 	carry	r16,{O,IO,IO,I}
   4:	29840426 	add	r12,r4,r6	; {O}
   8:	29a50429 	add	r13,r5,r9	; {IO}
   c:	29c6042a 	add	r14,r6,r10	; {IO}
  10:	29e7042b 	add	r15,r7,r11	; {I}
  14:	28220423 	add	r1,r2,r3
  18:	f2010002 	carry	r16,{O}
  1c:	29e7042b 	add	r15,r7,r11	; {O}
  20:	f3610001 	carry	r27,{I}
  24:	28220423 	add	r1,r2,r3	; {I}
