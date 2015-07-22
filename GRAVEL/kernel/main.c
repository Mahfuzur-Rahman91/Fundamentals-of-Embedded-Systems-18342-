/*                                                                    
 * main.c : Kernel main (entry) function                            
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <lock.h>

/* Custom defines */
#define USER_STACK_START 0xA2FFFFFC
#define SWI_VECTOR_TABLE_ADDRESS (unsigned*)0x08
#define SWI_JUMP_ADDRESS_MASK (unsigned)0x00000FFF
#define LOAD_INSTRUCTION_MASK (unsigned)0xfffff000
#define LOAD_INSTRUCTION_OPCODE (unsigned)0xe59ff000

#define IRQ_VECTOR_TABLE_ADDRESS (unsigned*)0x18
#define IRQ_JUMP_ADDRESS_MASK (unsigned)0x00000FFF
#define MASK26TH (unsigned)0x04000000	
#define COUNTZERO (unsigned)0x00000000
#define TIMESTEP 32500 //there would be 32500 count-ups for 10ms which is minimum time-step
#define HANDLER1ST (unsigned)0xe51ff004

/* Error codes */
#define BADCODE 0x0badc0de

uint32_t global_data;

/* Declaring global variables and functions */
void S_Handler(void); 
void irq_wrapper(void); 

int  modeswitch(void);
void swi_exit(int);

unsigned swi_handler_backup1;
unsigned swi_handler_backup2;
unsigned irq_handler_backup1;
unsigned irq_handler_backup2;

unsigned user_sp;
unsigned irq_sp = 0xa3ffffff;
volatile unsigned long timer_count;

unsigned saved_sp;
unsigned saved_lr;

/*------------------------------------------------                                
Function: main.c                                                         
Description:                                                                    
Kernel's kmain function.                                  
Input arguments:                                                                
The usual argc, argc and table variables.                                                                     
------------------------------------------------ */ 

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();

	global_data = table;
	timer_count = 0;
	dev_init();
	mutex_init();

	/* SWI variables */
	unsigned swi_vector_instruction; /* Vector table instruction*/ 
	unsigned imm12_swi; 
	unsigned *swi_handler_address; 
	unsigned swi_vector_instruction_masked;

    /* IRQ variables */
	unsigned irq_vector_instruction; /* Vector table instruction*/ 
	unsigned imm12_irq; 
	unsigned *irq_handler_address; 
	unsigned irq_vector_instruction_masked;

	int i = 0;
	int retval = 0;

	/* Saving argc and argv values onto the stack */
	unsigned *sp = (unsigned* )USER_STACK_START;
	*sp = 0;	
	sp  = (sp-1);

	for (i = (argc-1); i>=0; i--) {
		*sp = (unsigned) argv[i];
		sp  = sp-1;		
	} 
	*sp     = (unsigned) argc;
	sp 		= (sp-1);
	user_sp = (unsigned) sp;
		
	/* -------------- SWI STUFF ------------------------*/
	/* Masking the LDR opcode to get value for #imm12 */
	swi_vector_instruction 	= *SWI_VECTOR_TABLE_ADDRESS;
	imm12_swi    			= 0x08 + 0x08 + (swi_vector_instruction & SWI_JUMP_ADDRESS_MASK);
	swi_handler_address 	= (unsigned*) (*((unsigned*) imm12_swi));

    /* Checking if imm12 is negative and that 0x08 has a load instruction*/ 
    swi_vector_instruction_masked  = swi_vector_instruction & LOAD_INSTRUCTION_MASK;
    if (((int)imm12_swi < 0) || (swi_vector_instruction_masked != LOAD_INSTRUCTION_OPCODE)){                     
        return(BADCODE);                                               
    }                                                                       
                                
	/* Backing up original opcodes for restoration while returning */
	swi_handler_backup1 = *(swi_handler_address); 
	swi_handler_backup2 = *(swi_handler_address+1); 

	/* Overwriting the opcodes at the original SWI_Handler */
	*(swi_handler_address) 	 = HANDLER1ST; //machine code for mov pc, pc, #-4
	*(swi_handler_address+1) = (unsigned) S_Handler;

	/* -------------- IRQ STUFF ------------------------*/
    /* Setting up timer registers */
	reg_write(OSTMR_OSMR_ADDR(0), TIMESTEP);
	reg_set(OSTMR_OIER_ADDR ,  OSTMR_OIER_E0);
	reg_set(INT_ICMR_ADDR ,   MASK26TH);
	reg_clear(INT_ICLR_ADDR ,  MASK26TH);
	reg_write(OSTMR_OSCR_ADDR, COUNTZERO);

	irq_vector_instruction 	= *IRQ_VECTOR_TABLE_ADDRESS;
	imm12_irq    			= 0x08 + 0x18 + (irq_vector_instruction & IRQ_JUMP_ADDRESS_MASK);
	irq_handler_address 	= (unsigned*) (*((unsigned*) imm12_irq));

    /* Checking if imm12 is negative and that 0x18 has a load instruction*/ 
    irq_vector_instruction_masked  = irq_vector_instruction & LOAD_INSTRUCTION_MASK;
    if (((int)imm12_irq < 0) || (irq_vector_instruction_masked != LOAD_INSTRUCTION_OPCODE)){                     
        return(BADCODE);                  
        printf("error in irq \n") ;                            
    }                                                                       

	/* Backing up original opcodes for restoration while returning */
	irq_handler_backup1 = *(irq_handler_address); 
	irq_handler_backup2 = *(irq_handler_address+1); 

	/* Overwriting the opcodes at the original IRQ_Handler */
	*(irq_handler_address) 	 = HANDLER1ST; 
	*(irq_handler_address+1) = (unsigned) irq_wrapper;

	/* -------------- CALLING USER CODE ------------------------*/
	retval = modeswitch();

	/* Restoring original values of first two swi_instructions */
	*(swi_handler_address)   = swi_handler_backup1;
	*(swi_handler_address+1) = swi_handler_backup2;

	/* Restoring original values of first two irq_instructions */
	*(irq_handler_address)   = irq_handler_backup1;
	*(irq_handler_address+1) = irq_handler_backup2;
	return retval;

	assert(0);        /* should never get here */
}
