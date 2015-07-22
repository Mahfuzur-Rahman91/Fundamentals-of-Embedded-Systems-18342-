/** @file write.S
 *
 * @brief write sycall wrapper
 *
 * @author Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:46 -0400
 *
 * @author Sudhir Vijay <svijay@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>

	.file "write.S"

FUNC(write)
	stmfd	sp!, {r4, r5, r6} @Saving registers onto stack since SWI calls may change them
	swi WRITE_SWI @Calling SWI for write
	cmp r0, #0    @Checking for error values
	bge .L1	      @If no error, jump to end
	ldr r1, =errno @If error, get location of 'errno'
	rsb r0, r0, #0 @Convert negative return value to r0 by subtracting from zero.
	str r0, [r1]   @Store the positive error number.
	mvn r0, #0
.L1:
	ldmfd	sp!, {r4, r5, r6} @Restoring saved registers.
	mov pc, lr
