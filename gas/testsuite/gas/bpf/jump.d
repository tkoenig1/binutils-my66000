#as: -EL -mdialect=normal
#objdump: -dr -M dec
#source: jump.s
#name: eBPF JUMP instructions, normal syntax

.*: +file format .*bpf.*

Disassembly of section .text:

0+ <.text>:
   0:	05 00 03 00 00 00 00 00 	ja 3
   8:	0f 11 00 00 00 00 00 00 	add %r1,%r1
  10:	15 03 01 00 03 00 00 00 	jeq %r3,3,1
  18:	1d 43 00 00 00 00 00 00 	jeq %r3,%r4,0
  20:	35 03 fd ff 03 00 00 00 	jge %r3,3,-3
  28:	3d 43 fc ff 00 00 00 00 	jge %r3,%r4,-4
  30:	a5 03 01 00 03 00 00 00 	jlt %r3,3,1
  38:	ad 43 00 00 00 00 00 00 	jlt %r3,%r4,0
  40:	b5 03 01 00 03 00 00 00 	jle %r3,3,1
  48:	bd 43 00 00 00 00 00 00 	jle %r3,%r4,0
  50:	45 03 01 00 03 00 00 00 	jset %r3,3,1
  58:	4d 43 00 00 00 00 00 00 	jset %r3,%r4,0
  60:	55 03 01 00 03 00 00 00 	jne %r3,3,1
  68:	5d 43 00 00 00 00 00 00 	jne %r3,%r4,0
  70:	65 03 01 00 03 00 00 00 	jsgt %r3,3,1
  78:	6d 43 00 00 00 00 00 00 	jsgt %r3,%r4,0
  80:	75 03 01 00 03 00 00 00 	jsge %r3,3,1
  88:	7d 43 00 00 00 00 00 00 	jsge %r3,%r4,0
  90:	c5 03 01 00 03 00 00 00 	jslt %r3,3,1
  98:	cd 43 00 00 00 00 00 00 	jslt %r3,%r4,0
  a0:	d5 03 01 00 03 00 00 00 	jsle %r3,3,1
  a8:	dd 43 00 00 00 00 00 00 	jsle %r3,%r4,0
  b0:	06 00 00 00 01 00 00 00 	jal 1
  b8:	06 00 00 00 00 00 00 00 	jal 0