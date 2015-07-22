/*                                                                    
 * C_SWI_Handler.c: Kernel main (entry) function                            
 * Author:  Sudhir Kumar Vijay <svijay@andrew.cmu.edu>   
 *          Sumanth Suresh <sumanths@andrew.cmu.edu>             
 * Date:    11/2/2014                                               
 */          

#include <exports.h>
#include <bits/swi.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/time.h>
#include <syscall.h>
#include <config.h>
#include <kernel.h>
#include <sched.h>
#include <lock.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

#define step_count 10
#define BADCODE 0x0badc0de

void write(unsigned *regs){
    regs[0] = write_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);                                                 
}                                                                               

void read(unsigned *regs){                                                      
    regs[0] = read_syscall((int)regs[0], (void *)regs[1], (size_t)regs[2]);         
}

void  time_swi(unsigned *regs) {
    *(unsigned long*) regs = time_syscall();
}

void sleep_swi(unsigned long millis) {
    sleep_syscall(millis);    
}

/* 
------------------------------------------------
Function: C_SWI_Handler 
Description:
The SWI handler is called by the S_Handler assembly
code. Blocking interrupts for tasks related to context
switching/premption.
Input arguments: 
unsigned swi_num - Extracted SWI number from original
SWI call.
unsigned *regs   - Location of saved arguments from 
the caller.
------------------------------------------------
*/
void C_SWI_Handler(unsigned swi_num, unsigned *regs){                         
	switch(swi_num){                                                        
        case READ_SWI:                                                  
       		read(regs);                                             
            break;                                                  
        case WRITE_SWI:                                                 
            write(regs);                                            
            break;                                                  
	    case TIME_SWI:
		    time_swi(regs);
		    break;
	    case SLEEP_SWI:
		    sleep_swi(*((unsigned long *)regs));
		    break;           
        case CREATE_SWI:
            disable_interrupts();
            regs[0] = task_create((task_t*) regs[0], (size_t)regs[1]);         
            break;                                                  
        case EVENT_WAIT:
            disable_interrupts();
            regs[0] = event_wait((unsigned int) regs[0]);
            break;     
        case MUTEX_CREATE:
            disable_interrupts();
            regs[0] = mutex_create();
            break;                                                                                      
        case MUTEX_LOCK:
            disable_interrupts();
            regs[0] = mutex_lock(*(int*)regs);
            break;                                                                                      
        case MUTEX_UNLOCK:
            disable_interrupts();
            regs[0] = mutex_unlock(*(int*)regs);
            break;     
        default:
            regs[0] = BADCODE;
            invalid_syscall(swi_num);
            break;                                                                         
	}
}     
