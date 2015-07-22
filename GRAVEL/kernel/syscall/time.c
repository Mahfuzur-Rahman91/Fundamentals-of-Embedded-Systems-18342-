/** @file proc.c
 * 
 * @brief Implementation of timer syscalls
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <types.h>
#include <config.h>
#include <bits/errno.h>
#include <arm/timer.h>
#include <syscall.h>
#include <exports.h>
#include <bits/swi.h>
#include <bits/fileno.h>
#include <syscall.h>

#define step_count 10
extern volatile unsigned long timer_count; // Global counter.

/*
------------------------------------------------                                
time_syscall 
------------------------------------------------                                
Returns time in milliseconds since user kernel start
*/
unsigned long time_syscall(void)
{
   return (unsigned long)(timer_count*step_count);
}


/*
------------------------------------------------                                
sleep_syscall 
------------------------------------------------                                
Waits in a tight loop for atleast the given number of milliseconds.

Input parameters: millis  
The number of milliseconds to sleep.
*/
void sleep_syscall(unsigned long millis  __attribute__((unused)))
{
	unsigned long old_count = (timer_count*step_count);
	while(((timer_count*step_count)-old_count) < millis){
	}
}
