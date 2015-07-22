/** @file mutex_unlock.S
 *
 * @brief mutex unlock sycall wrapper
 *
 * @author Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @	   Sudhir Vijay <svijay@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_unlock.S"
	.text
	.global mutex_unlock
	.extern errno

mutex_unlock:
	stmfd sp!, {r4, r5, r6}
	swi MUTEX_UNLOCK @swi call to mutex_unlock is made here
	cmp r0, #0
	bge .L1
	rsb r0, r0, #0 @making the return value positive if error
	ldr r1, =errno
	str r0, [r1]
	mvn r0, #0
.L1:
	ldmfd sp!, {r4, r5, r6}
	mov pc, lr

