/**
 * @file mutex.c
 *
 * @brief Implements mutex functions.
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

//#define DEBUG_MUTEX
#include <lock.h>
#include <task.h>
#include <sched.h>

#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> 
#endif

mutex_t gtMutex[OS_NUM_MUTEX]; 
static tcb_t* rear[OS_NUM_MUTEX]; // Array to hold tails of sleep queues
static int mutexid = 0; // Mutexid returned to user.
 
/*                                                                              
------------------------------------------------                                
Function: mutex_init                                                         
Description:                                                                    
Used to initialize the mutex data structures.
------------------------------------------------                                
*/     
void mutex_init()
{
	int i;
	for (i=0; i<OS_NUM_MUTEX; ++i) {
		gtMutex[i].bAvailable = 0;
		gtMutex[i].pHolding_Tcb = 0;		
		gtMutex[i].bLock = 0;
		gtMutex[i].pSleep_queue = 0;
		rear[i] = 0;	
	}
}

/*                                                                              
------------------------------------------------                                
Function: mutex_create                                                         
Description:                                                                    
Creates a mutex and returns mutexid to user
------------------------------------------------                                
*/     
int mutex_create(void)
{
	mutexid++;
	if (mutexid > OS_NUM_MUTEX-1)
		return -ENOMEM;
	gtMutex[mutexid-1].bAvailable = 1;
	return mutexid;
}

/*                                                                              
------------------------------------------------                                
Function: mutex_lock                                                         
Description:                                                                    
Locks the mutex if no process currently holds the 
mutex, else puts the current process in sleep queue.
------------------------------------------------                                
*/     
int mutex_lock(int mutex)
{	
	tcb_t* cur_tcb;

	/*Getting currrently running TCB*/
	cur_tcb = get_cur_tcb();		
	
	if (mutex<1 || mutex>OS_NUM_MUTEX || (gtMutex[mutex-1].bAvailable!=1)) {
		return -EINVAL;
	}
	if (gtMutex[mutex-1].pHolding_Tcb == cur_tcb) {
		return -EDEADLOCK;
	}

	/*Locking the current mutex, if available*/
	if (gtMutex[mutex-1].bLock == 0) {
		gtMutex[mutex-1].pHolding_Tcb = cur_tcb;
		gtMutex[mutex-1].bLock = 1;
		gtMutex[mutex-1].pHolding_Tcb->holds_lock = 1;
		cur_tcb->cur_prio = 0;		
		return 0;
	}
	/* adding process to mutex sleep queue */
	else {		
		/* If initial sleep queue is empty*/		
		if (gtMutex[mutex-1].pSleep_queue == 0){
			cur_tcb->sleep_queue = 0;
			gtMutex[mutex-1].pSleep_queue = cur_tcb;
			rear[mutex-1] = cur_tcb;
		} 
		else {
			/* Attaching current head to new del_tcb and updating head*/
			cur_tcb->sleep_queue = 0;
			rear[mutex-1]->sleep_queue = cur_tcb;
			rear[mutex-1] = cur_tcb;
		}	
		dispatch_sleep();		
		return 0; 
	}
	return 0;
}

/*                                                                              
------------------------------------------------                                
Function: mutex_unlock                                                         
Description:                                                                    
Unlocks the mutex and assigns lock to next-in-queue.
If queue is empty, it simply unlocks the mutex
------------------------------------------------                                
*/     
int mutex_unlock(int mutex)
{
	tcb_t* curr_tcb;
	tcb_t* temp_tcb;

	curr_tcb = get_cur_tcb();

	if (mutex<1 || (gtMutex[mutex-1].bAvailable!=1)){
		return -EINVAL;
	}

	if(curr_tcb != gtMutex[mutex-1].pHolding_Tcb){
		return -EPERM;
	}

	gtMutex[mutex-1].pHolding_Tcb->holds_lock = 0;
	gtMutex[mutex-1].bLock = 0;
	curr_tcb->cur_prio = curr_tcb->native_prio;		

	/*Changing lock status in current_tcb*/
	if(gtMutex[mutex-1].pSleep_queue!=0){
		/* Updating status of next in line for lock*/
		/* If queue is not single-element */
		runqueue_add(gtMutex[mutex-1].pSleep_queue, gtMutex[mutex-1].pSleep_queue->native_prio);
		if(gtMutex[mutex-1].pSleep_queue->sleep_queue!=0){
			temp_tcb = gtMutex[mutex-1].pSleep_queue;
			gtMutex[mutex-1].pSleep_queue = temp_tcb->sleep_queue;
			temp_tcb->sleep_queue = 0;
		}
		else {
			/* If queue is single-element */
			temp_tcb = gtMutex[mutex-1].pSleep_queue;
			gtMutex[mutex-1].pSleep_queue = 0;
			temp_tcb->sleep_queue = 0;
			rear[mutex-1] = 0;		
		}
		/* Updating status of mutex */
		gtMutex[mutex-1].bLock = 1;
		gtMutex[mutex-1].pHolding_Tcb = temp_tcb;
		gtMutex[mutex-1].pHolding_Tcb->holds_lock = 1;		
	}
	else {
		/* If sleep queue is empty, unlock mutex*/
		gtMutex[mutex-1].bLock = 0;
		gtMutex[mutex-1].pHolding_Tcb = 0;
	}
	return 0; 
}

