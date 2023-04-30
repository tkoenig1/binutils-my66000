#name: Enter and exit
#objdump: -d

.*: +file format .*

Disassembly of section .text:

0000000000000000 <.text>:
   0:	b0220100 	enter	r1,r2,256
   4:	b1ef0401 	enter	r15,r15,1024,1
   8:	b2311002 	enter	r17,r17,4096,2
   c:	b0221003 	enter	r1,r2,4096,3
  10:	9c220100 	exit	r1,r2,256
  14:	9def0401 	exit	r15,r15,1024,1
  18:	9e311002 	exit	r17,r17,4096,2
  1c:	9c221003 	exit	r1,r2,4096,3
