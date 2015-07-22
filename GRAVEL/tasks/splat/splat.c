/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 * Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 * Sumanth Suresh <sumanths@andrew.cmu.edu>
 * Links to libc.
 */
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int delay = 200;
	while(1){
		printf("|");
    	sleep(delay);
    	printf("\b");
    	printf("/");
    	sleep(delay);
    	printf("\b");
    	printf("-");
    	sleep(delay);
    	printf("\b");	
    	printf("\\");
     	printf("\b");	
	}
	return 0;
}
