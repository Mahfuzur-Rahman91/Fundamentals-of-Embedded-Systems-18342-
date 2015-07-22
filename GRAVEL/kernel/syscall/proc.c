/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <lock.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

#define USER_STACK_START 0xA2FFFFFC

/*
------------------------------------------------                                
task_create syscall
------------------------------------------------                                
Used to create and allocate user tasks. Returns error
values incase call fails.
*/
int task_create(task_t* tasks, size_t num_tasks)
{   
	int i, j,ret_sch = 0;
	task_t temp;
	
	if (num_tasks > OS_MAX_TASKS){
		return -EINVAL;
	}

	if ((unsigned)tasks > (unsigned) USER_STACK_START){
		return -EFAULT;
	}
	
	// Insertion sort of tasks - according to cur_prio 
	for (i=1; i<(int) num_tasks; ++i) {
		j = i-1;
		temp = tasks[i];
		while(j>=0 && (tasks[j].T > temp.T)) {
			tasks[j+1] = tasks[j];
			j--;
		}
		tasks[j+1] = temp;	
	}		

	for (i=0; i<(int) num_tasks; i++) {
		if (tasks[i].C > tasks[i].T) 
			return -ESCHED;		
	}

    ret_sch = assign_schedule(&tasks , num_tasks);

    /* If tasks cannot be scheduled, delete return -1 */
    if(ret_sch==0){
    	printf("UBTEST FAILED: Tasks cannot be scheduled !\n");
    	return -1;
    }
    
    /* If tasks can be scheduled, delete return -1 */
	allocate_tasks(&tasks, num_tasks);
	return 0;
}


/*
------------------------------------------------                                
event_wait syscall
------------------------------------------------                                
Implements event_wait on a particular
devices - calls dev_wait.
*/
int event_wait(unsigned int dev)
{
	tcb_t* curr_tcb;
  	if(dev>NUM_DEVICES-1){
  		return -EINVAL;
  	}

  	/* Returns -EHOLDSLOCK, if mutex process calls event_wait */
	curr_tcb = get_cur_tcb();
  	if (curr_tcb->cur_prio == 0){
 		return -EHOLDSLOCK; 	
  	}
  	
  	dev_wait(dev);
  	return 0; 
}

/*
------------------------------------------------                                
invalid_syscall 
------------------------------------------------                                
An invalid syscall causes the kernel to exit.
*/
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
