/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */
#include <types.h>
#include <assert.h>
#include <kernel.h>
#include <sched.h>
#include "sched_i.h"

static tcb_t* run_list[OS_MAX_TASKS]  __attribute__((unused));

/*
------------------------------------------------                                
run_bits 
------------------------------------------------                                
A high bit in this bitmap means that the task whose priority is
equal to the bit number of the high bit is runnable.
*/

static uint8_t run_bits[OS_MAX_TASKS/8] __attribute__((unused));

/*
------------------------------------------------                                
group_run_bits 
------------------------------------------------                                
This is a trie structure.  Tasks are grouped in groups of 8.  If any task
in a particular group is runnable, the corresponding group flag is set.
Since we can only have 64 possible tasks, a single byte can represent the
run bits of all 8 groups.
*/

static uint8_t group_run_bits __attribute__((unused));


/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[]  __attribute__((unused)) =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
	int i;	
	group_run_bits = 0;
	for (i=0; i<=7; ++i) {
		run_bits[i] = 0;
	}	
}

/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb  __attribute__((unused)), uint8_t prio  __attribute__((unused)))
{
	uint8_t Prio = tcb -> cur_prio;
	uint8_t ostcby = Prio >>3;
	uint8_t ostcbx = Prio & 0x07;
	
	run_bits[ostcby] = (uint8_t)(1<<ostcbx) | run_bits[ostcby];
	group_run_bits |= (uint8_t)(1<<ostcby);		
}

/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio  __attribute__((unused)))
{
	uint8_t ostcby = prio >>3;
	uint8_t ostcbx = prio & 0x07;
	tcb_t* ret_tcb = 0;
	if(prio == 63){
		ret_tcb = &system_tcb[prio];
	} else{
		run_bits[ostcby] = (uint8_t)(1<<ostcbx)^run_bits[ostcby];
		if (run_bits[ostcby]==0) {
			group_run_bits ^= (uint8_t)(1<<ostcby);
		}
		ret_tcb = &system_tcb[prio];
	}
	return (tcb_t *)ret_tcb; 
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{	
	uint8_t y = prio_unmap_table[group_run_bits];
	uint8_t x = prio_unmap_table[run_bits[y]];
	uint8_t prio = (y<<3) + x;
 	return prio;	
}
