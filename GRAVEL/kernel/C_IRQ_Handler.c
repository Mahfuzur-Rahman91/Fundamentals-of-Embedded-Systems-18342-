/*                                                                    
 * C_IRQ_Handler.c: IRQ Handler                          
 * Author: Sudhir Kumar Vijay <svijay@andrew.cmu.edu>     
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>           
 * Date:   11/2/2014                                                
 */          

#include <exports.h>
#include <bits/swi.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <arm/time.h>
#include <types.h>
#include <assert.h>
#include <device.h>
#include <kernel.h>
#include <config.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

#define COUNT0 0x00000000
#define step_count 10

void C_IRQ_Handler(){        
	/* Incrementing global time_count */
	/* This has a granularity of 10ms */
    timer_count++;
    reg_set(OSTMR_OSSR_ADDR, 	OSTMR_OSSR_M0);
    reg_write(OSTMR_OSCR_ADDR, 	COUNT0);
    dev_update(timer_count*step_count);
}     
