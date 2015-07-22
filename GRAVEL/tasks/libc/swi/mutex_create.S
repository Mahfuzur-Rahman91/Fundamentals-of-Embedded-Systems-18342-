/** @file mutex_create.S
 *
 * @brief mutex create sycall wrapper
 *
 * @author Sumanth Suresh <sumanths@andrew.cmu.edu>
 * 	   Sudhir Vijay <svijay@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <bits/swi.h>
#include <asm.h>
	.file	"mutex_create.S"
	.text

	.global	mutex_create
	.extern errno
mutex_create:
	stmfd	sp!, {r4, r5, r6} @Saving registers onto stack since SWI calls may change them
	swi MUTEX_CREATE @Doing a system call for mutex create
	cmp r0, #0   @Compare return value with 0 to check for errors
	bge .L1      @If no error => branch out 		
	rsb r0, r0, #0 @If error, getting the negative value of the error value by subtracting from zero.
	ldr r1, =errno
	str r0, [r1] @Putting the error in the external variable 'errno'	
	mvn r0, #0 @moving -1 to r0
.L1:
	ldmfd	sp!, {r4, r5, r6} @Restoring the registers.
	mov pc, lr
