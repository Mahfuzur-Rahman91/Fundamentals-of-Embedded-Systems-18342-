@ I_Handler.S: Kernel startup routine
@ Author: 	Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@			Sumanth Suresh <sumanths@andrew.cmu.edu>
@ Date:   	11/2/2014 
@ Interrupt Handler

	.file	"I_Handler.S"
	.text
	.global I_Handler
	.extern irq_sp				@global irq_sp from C file.
I_Handler:
	LDR r9, =irq_sp
	LDR sp, [r9]				@Setting sp to safe location in RAM
	SUB	sp, sp, #4
	STMFD	sp!, {r0-r12, lr}	@Storing context registers
	MRS	r2, spsr				
	STR	r2, [sp, #14*4]		
	MOV	r1, sp
	BL	C_IRQ_Handler			@Branching to C_IRQ_Handler
	LDR	r2, [sp, #14*4]
	MSR	spsr, r2
	LDMFD	sp!, {r0-r12, lr}
	ADD	sp, sp, #4
	MOVS	pc, lr
