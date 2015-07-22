/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <bits/swi.h>
#include <asm.h>
	.file	"read.S"
	.text

	.global	read
	.extern errno
read:
	stmfd	sp!, {r4, r5, r6} @Saving registers onto stack since SWI calls may change them
	swi READ_SWI @Doing a system call for read
	cmp r0, #0   @Compare return value with 0 to check for errors
	bge .L1      @If no error => branch out 		
	rsb r0, r0, #0 @If error, getting the negative value of the error value by subtracting from zero.
	ldr r1, =errno
	str r0, [r1] @Putting the error in the external variable 'errno'	
	mvn r0, #0 @moving -1 to r0
.L1:
	ldmfd	sp!, {r4, r5, r6} @Restoring the registers.
	mov pc, lr
