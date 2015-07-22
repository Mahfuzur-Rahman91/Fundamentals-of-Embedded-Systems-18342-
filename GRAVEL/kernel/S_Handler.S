@ start.S: 	Kernel startup routine
@ Author: 	Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@	  		Sumanth Suresh <sumanths@andrew.cmu.edu>
@ Date:   	5th November, 2014 
	.file	"S_Handler.S"
	.text
	.global S_Handler
	.extern global_data
S_Handler:
	SUB	sp, sp, #4
	STMFD	sp!, {r0-r12, lr}
	MRS	r2, spsr
	STR	r2, [sp, #14*4]
	STMFD	sp, {sp, lr}^ @Storing user stack and link registers
	ADD	sp, sp, #-8
	MOV	r1, sp
	ADD 	r1, r1, #8
	LDR	r0, [lr, #-4]
	BIC	r0, r0, #0xff000000
	MRS	r9, cpsr
	AND	r9, r9, #0xFFFFFF7F  @Enabling interrupts by resetting the 8th bit of the cpsr
	MSR	cpsr, r9
	LDR	r9, =global_data  @restoring r8 with start of uboot function table
	LDR	r8, [r9]
	BL	C_SWI_Handler
	LDMFD	sp, {sp, lr}^
	ADD	sp, sp, #8	
	LDR	r2, [sp, #14*4]
	MSR	spsr, r2
	LDMFD	sp!, {r0-r12, lr}
	ADD	sp, sp, #4
	MOVS	pc, lr
