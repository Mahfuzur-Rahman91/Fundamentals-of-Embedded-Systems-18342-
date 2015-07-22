/** @file typo.c
 * Authors: 	Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 * 				Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @brief:	 	Program displays a diamond pattern folding and
 *				unfolding by half after a certain interval.
 * Links to libc.
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define ESC 27

int main(int argc, char** argv){
   printf("***********************************************\n");
   printf("******** Diamond Folding and Unfolding ********\n");
   printf("***********************************************\n");

   while(1){	
	printf("  *\n ***\n*****\n ***\n  *");
	sleep(1000);
	printf("%c[4A\r", ESC); // Move cursor up 4 rows and return carriage
	printf("  *\n ***\n*****\n    \n   ");
	sleep(1000);
	printf("%c[4A\r", ESC);
	printf("  *\n ** \n***  \n    \n   ");
	sleep(1000);
	printf("%c[4A\r", ESC);
	printf("  *\n ***\n*****\n    \n   ");
    printf("%c[4A\r", ESC);
	sleep(1000);
	printf("  *\n ***\n*****\n ***\n  *");
    printf("%c[4A\r", ESC);
	sleep(1000);
  }	
   return 0;
}	 
