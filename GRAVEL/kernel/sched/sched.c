/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <types.h>
#include <assert.h>
#include <kernel.h>
#include <config.h>
#include "sched_i.h"
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

/* Giving random safe SP for the idle stack */ 
#define IDLE_SP 0xa2500000

void sched_init(task_t* main_task  __attribute__((unused)))
{
	/* unused */
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
static void idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already bceen deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 * - TCB[0] has been left empty for part-2. So, task-0 is assigned to TCB-1, and
 *   so on.
 * @param tasks  A list of scheduled task desbcriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
	task_t* task_ptr = (task_t*)*tasks;
	unsigned i = 0;

	/* Initializing TCBs */
	for (i = 0; i<OS_MAX_TASKS; i++){
		system_tcb[i].cur_prio    = 0;
		system_tcb[i].native_prio = 0;		
	   	system_tcb[i].sleep_queue = 0;   
	   	system_tcb[i].holds_lock  = 0;   
	}

	/* Allocating TCB-0 */
	system_tcb[0].native_prio = 0;
	system_tcb[0].cur_prio    = 0;
	system_tcb[0].context.r4  = 0;
    system_tcb[0].context.r5  = 0;
    system_tcb[0].context.r6  = IDLE_SP;
	system_tcb[0].context.sp  = &system_tcb[0].kstack_high[0];	   	
   	system_tcb[0].context.lr  = 0;
	system_tcb[0].sleep_queue = 0;   
	system_tcb[0].holds_lock  = 0;   

	/* Setting up TCB for all tasks*/
	for (i = 0; i<num_tasks; i++){
		system_tcb[i+1].native_prio = i+1;
		system_tcb[i+1].cur_prio = i+1;
    	system_tcb[i+1].context.r4  = (unsigned)(task_ptr[i].lambda);
    	system_tcb[i+1].context.r5  = (unsigned)(task_ptr[i].data);
    	system_tcb[i+1].context.r6  = (unsigned)(task_ptr[i].stack_pos);
	   	system_tcb[i+1].context.sp  = &system_tcb[i].kstack_high[0];	   	
	   	system_tcb[i+1].context.lr  = (void*)0;   
	   	system_tcb[i+1].sleep_queue = 0;   
	   	system_tcb[i+1].holds_lock  = 0;   
        runqueue_add(&system_tcb[i+1], (uint8_t) i+1);
	}

	/* Setting up TCB for idle task */
	system_tcb[63].native_prio = 63;
	system_tcb[63].cur_prio = 63;
	system_tcb[63].context.r4  = (unsigned) idle;
    system_tcb[63].context.r5  = 0;
    system_tcb[63].context.r6  = IDLE_SP;
	system_tcb[63].context.sp  = &system_tcb[63].kstack_high[0];	   	
   	system_tcb[63].context.lr  = (void*)0;
	system_tcb[63].sleep_queue = 0;   
	system_tcb[63].holds_lock  = 0;   
    runqueue_add(&system_tcb[63], (uint8_t) 63);

    /* Calling dispatch nosave */
    dispatch_nosave();
}

