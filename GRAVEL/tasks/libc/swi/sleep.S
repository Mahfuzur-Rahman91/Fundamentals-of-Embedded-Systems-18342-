/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <bits/swi.h>
#include <asm.h>

	.file "sleep.S"

FUNC(sleep)
	stmfd	sp!, {r4, r5, r6} @Saving registers onto stack since SWI calls may change them
	swi 	SLEEP_SWI @Doing a system call for read
	ldmfd	sp!, {r4, r5, r6} @Restoring the registers.
	mov pc, lr
