/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>
	
	.file "time.S"

FUNC(time)
	stmfd	sp!, {r4, r5, r6} 	@Saving registers onto stack
	swi 	TIME_SWI 			@Doing a system call for time
	ldmfd	sp!, {r4, r5, r6} 	@Restoring the registers.
	mov pc, lr

