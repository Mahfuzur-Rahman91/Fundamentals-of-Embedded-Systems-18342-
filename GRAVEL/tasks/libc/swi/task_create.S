/** @file task_create.S
 *
 * @brief task create wrapper
 *
 * @author Sumanth Suresh <sumanths@andrew.cmu.edu>
 * 	   Sudhir Vijay <sumanths@andrew.cmu.edu> 
 * @date 2008-10-31
 */

#include <bits/swi.h>
#include <asm.h>

	.file	"task_create.S"
	.text

	.global	task_create
	.extern errno
task_create:
	stmfd	sp!, {r4, r5, r6} @Saving registers onto stack since SWI calls may change them
	swi CREATE_SWI @Calling SWI for task_create
	cmp r0, #0    @Checking for error values
	bge .L1	      @If no error, jump to end
	ldr r1, =errno @If error, get location of 'errno'
	rsb r0, r0, #0 @Convert negative return value to r0 by subtracting from zero.
	str r0, [r1]   @Store the positive error number.
	mvn r0, #0
.L1:	
	ldmfd	sp!, {r4, r5, r6} @Restoring saved registers.
	mov pc, lr
