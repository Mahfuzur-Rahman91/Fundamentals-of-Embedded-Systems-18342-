@ crt0.S: Execution startup routine
@
@ Authors: Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@ Date:    4th October 2014 
#include <bits/swi.h>
	.file	"crt0.S"
	.text
	.global	_start
_start:
	ldr r0, [sp]
	add r1, sp, #4
	bl main
