 /** @file ctx_switch.c
 * 
 * @brief Implementation of despatch functions.
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <types.h>
#include <assert.h>
#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static __attribute__((unused)) tcb_t* cur_tcb; /* use this if needed */

/*
------------------------------------------------                                
dispatch_init 
------------------------------------------------                                
Set the initialization thread's priority to IDLE so that anything
will preempt it when dispatching the first task.
*/
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
		/* Not needed ?*/

}

/*
------------------------------------------------                                
dispatch_save 
------------------------------------------------                                
Context switch to the highest priority task while saving off the 
current task state.

This function needs to be externally synchronized.

We could be switching from the idle task.  The priority searcher has been tuned
to return IDLE_PRIO for a completely empty run_queue case.
*/
void dispatch_save(void)
{
	tcb_t* run_tcb;
	tcb_t* current_tcb; 
	
	current_tcb = cur_tcb;
	runqueue_add(cur_tcb, cur_tcb->cur_prio);
	run_tcb = runqueue_remove(highest_prio());

	if(run_tcb != current_tcb){
		cur_tcb = run_tcb;
		ctx_switch_full(&(run_tcb->context), &(current_tcb->context));
	}
}

/*
------------------------------------------------                                
dispatch_save 
------------------------------------------------                                
Context switch to the highest priority task that is not
this task don't save the current task state.

There is always an idle task to switch to.
*/

void dispatch_nosave(void)
{
	tcb_t* run_tcb;
	run_tcb = runqueue_remove(highest_prio());
	cur_tcb = run_tcb;

	/* Context switch save to the new task after saving */
	ctx_switch_half(&(run_tcb->context));
}

/*
------------------------------------------------                                
dispatch_sleep 
------------------------------------------------                                
Context switch to the highest priority task that is not 
this task and save the current task but don't mark is 
runnable. There is always an idle task to switch to.
*/
void dispatch_sleep(void)
{
	tcb_t* run_tcb;
	tcb_t* current_tcb; 

	current_tcb = cur_tcb;
	run_tcb = runqueue_remove(highest_prio());
	
	/* Context switch save to the new task after saving */
	cur_tcb = run_tcb;
	ctx_switch_full(&(run_tcb->context), &(current_tcb->context));
}

/*
------------------------------------------------                                
get_cur_prio 
------------------------------------------------                                
Returns the priority value of the current task.
*/
uint8_t get_cur_prio(void)
{
	return (cur_tcb->cur_prio);
}

/*
------------------------------------------------                                
get_cur_prio 
------------------------------------------------                                
Returns the TCB of the current task.
*/
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
