@ Author: Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@ Date:   19th October, 2014
@ Assembly code that implements a swi_exit. 
        .file   "swi_exit.S"
        .text
        .global swi_exit
	.extern saved_sp
swi_exit:
        LDR     r9, =saved_sp     @Restoring stack pointer from global variable 
        LDR	sp, [r9]
	LDMFD   sp!, {r1-r12, lr} @Restoring state, except r0 which has the exit code.
	MOV	pc, lr		  @Jumping back to kernel mode
