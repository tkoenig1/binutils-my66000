#name: HR instruction
#objdump: -d
.*:     file format .*


Disassembly of section .text:

0+ <.text>:
   0:	36202000 	hr	r17,mrp
   4:	36202001 	hr	r17,masid
   8:	36202002 	hr	r17,crp
   c:	36202003 	hr	r17,casid
  10:	36202008 	hr	r17,trp
  14:	36202009 	hr	r17,tasid
  18:	3620200a 	hr	r17,csp
  1c:	36202018 	hr	r17,ip
  20:	36202019 	hr	r17,m
  24:	3620201a 	hr	r17,a
  28:	3620201b 	hr	r17,e
  2c:	3620201c 	hr	r17,s
  30:	3620201d 	hr	r17,f
  34:	3620201e 	hr	r17,rm
  38:	3620201f 	hr	r17,w
  3c:	34114018 	jmp	r17
  40:	34114019 	hr	m,r17
  44:	3411401a 	hr	a,r17
  48:	3411401b 	hr	e,r17
  4c:	3411401c 	hr	s,r17
  50:	3411401d 	hr	f,r17
  54:	3411401e 	hr	rm,r17
  58:	3411401f 	hr	w,r17
  5c:	34116018 	calli	r17
  60:	34206018 	hr	r1,ip,r0
  64:	34316019 	hr	r1,m,r17
  68:	36216019 	hr	r17,m,r1
  6c:	3631601a 	hr	r17,a,r17
  70:	3631601b 	hr	r17,e,r17
  74:	3631601c 	hr	r17,s,r17
  78:	3631601d 	hr	r17,f,r17
  7c:	36316019 	hr	r17,m,r17
  80:	3631601f 	hr	r17,w,r17
  84:	34114018 	jmp	r17
  88:	34116018 	calli	r17
