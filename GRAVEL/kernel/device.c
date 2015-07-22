/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <types.h>
#include <assert.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
/* Frequency allocation for stress test*/
//const unsigned long dev_freq[NUM_DEVICES] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900, 5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900, 9000};

/* Frequency allocation for other tests*/
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i = 0;
	for(i=0; i<NUM_DEVICES; i++) {
		devices[i].sleep_queue = 0;
		devices[i].next_match  = dev_freq[i];
	}
}

/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev)
{	
	tcb_t* del_tcb;
	/*Getting currrently running TCB*/
	del_tcb = get_cur_tcb();
	
	if (devices[dev].sleep_queue == 0){
		/* If initial sleep queue is empty*/
		del_tcb->sleep_queue = 0;
		devices[dev].sleep_queue = del_tcb;
	} else {
		/* Attaching curre2nt head to new del_tcb and updating head*/
		del_tcb->sleep_queue = devices[dev].sleep_queue;
		devices[dev].sleep_queue = del_tcb;
	}
    dispatch_sleep();
}

/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis)
{
    tcb_t* curr_tcb;
    int i= 0;

    /* Adding the device sleep-queue processes to run queue, in order of devices */
   	for (i=0; i<NUM_DEVICES; i++) {
    	if(millis >= devices[i].next_match){
    		for(curr_tcb = devices[i].sleep_queue; curr_tcb!=0; curr_tcb = curr_tcb->sleep_queue){
				runqueue_add(curr_tcb, curr_tcb->cur_prio);
			}
			/* Emptying the sleep_queue and updating the next_match*/
			devices[i].sleep_queue = 0;
			devices[i].next_match += dev_freq[i];
    	}
	}

	if(get_cur_prio() > highest_prio()){
		dispatch_save();
	}
}

