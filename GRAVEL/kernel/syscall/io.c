/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT_CHAR 0x04
#define DEL_CHAR 0x7f
#define SDRAM_LOW 0xa0000000
#define SDRAM_HIGH 0xa3ffffff
#define ROM_HIGH 0x00ffffff
#define ROM_LOW  0xa0000000

/*                                                                              
------------------------------------------------                                
Function: read                                                         
Description:                                                                    
Read count bytes (or less) from fd into the buffer buf.Input arguments:                                                                
unsigned *regs   - Location of saved arguments from                             
the caller.                                                                     
------------------------------------------------                                
*/                                                                              
ssize_t read_syscall(int fd, void *buf, size_t count)
{                                                                    
    char* buffer;                                                                                                                            
    unsigned int i = 0;                                                              
    char current_char = 0;                                                  
    buffer = (char* )buf;                                                                                                                                                                    
    if(fd!=STDIN_FILENO){                                                   
        return -EBADF;
	}                                                                       
    else if (((unsigned )buffer<SDRAM_LOW) || (((unsigned )buffer + count) > SDRAM_HIGH)) {  
		return -EFAULT;                                              
    }                   
    else {                                                                  
        for(i=0; i<count; i++){                                         
            current_char = getc();                                  
            if (current_char == 4 ){
                // Taking care of EOT                        
                break;                                          
            } else if ((current_char == 8) || (current_char == 127)){  
                // Taking care of backspace/delete
                if(i>0) {
					i = i - 2; 
                    puts("\b \b");                                  
                } 
                else {
			         i=0;
				}                                     
            } else if ((current_char == 10) || (current_char == 13)){
                // Taking care of newline/carriage-return
			 	buffer[i] = '\n';                                  
                putc('\n');                                     
                buffer[i+1] = '\0';
		        i = i + 1;    //Incrementing to account for newly written '\n'                                  
                break;                                          
            } else {                                                
                buffer[i] = current_char;                          
                putc(buffer[i]);                                   
            }                                                       
        }                            
    }                                                               
    return ((ssize_t)i);
}

/*                                                                              
------------------------------------------------                                
Function: write                                                                 
Description:                                                                    
Write function for the C SWI handler.                                            
Input arguments:                                                                
unsigned *regs   - Location of saved arguments from                             
the caller.                                                                     
------------------------------------------------                                
*/                                                                              
ssize_t write_syscall(int fd  __attribute__((unused)), const void *buf  __attribute__((unused)), size_t count  __attribute__((unused)))
{
    char* buffer;                                                                                                                            
    unsigned int i = 0;                                                              
                                                                                                                                   
    buffer  = (char* )buf;                                                                                                         	                                                                               
    unsigned max_bufsize = (unsigned )buffer + count;

    if(fd!=STDOUT_FILENO){                                                  
	   return -EBADF;                                            
    }              
    else if (((char*)buffer< (char*)ROM_LOW) || ((char*)max_bufsize>(char*)SDRAM_HIGH)) { 
        return -EFAULT;
    }                                                                       
	else if (((char*)buffer<(char*)SDRAM_LOW) && ((char*) max_bufsize>(char*)ROM_HIGH)) { 
        return -EFAULT;
	}                                                                       
    else {                                                                  
        for (i = 0; i<count; i++){                                      
            if(buffer[i]==10){                                         
                putc(buffer[i]);                                   
                i++;                                            
                break;                                          
            } else {                                                
               putc(buffer[i]);                                   
            }                                                       
        }                                                               
	}                          
   return ((ssize_t)i);
}

