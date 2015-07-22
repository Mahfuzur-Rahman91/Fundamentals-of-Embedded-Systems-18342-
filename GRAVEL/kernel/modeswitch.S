@ Authors: 	Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@			Sumanth Suresh <sumanths@andrew.cmu>
@ Date:   11/2/2014
@ Description: Module that changes mode from supervisor to user
	.file	"modeswitch.S"
	.text
	.global modeswitch
	@Global variables from C
	.extern saved_sp
	.extern user_sp
modeswitch:
	@Storing current state on stack
	STMFD	sp!, {r1-r12,lr}
	LDR	r9, =saved_sp @saving stack pointer to global variable
	STR	sp, [r9]

	@Changing to user mode
	MRS	r9, cpsr
	BIC	r9, r9, #31
	ORR	r9, r9, #80  @Enabling interrupts for IRQ mode
	MSR	cpsr, r9
	LDR	r9, =user_sp @user_sp is sp after pushing argc, argv values
	LDR	sp, [r9]

	@Calling user function.	
	LDR	pc, =0xa0000000
