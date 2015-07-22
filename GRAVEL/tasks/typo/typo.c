/** @file typo.c
 * Authors: Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 * 			Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include<stdio.h>
#include<unistd.h>

/* Separating mantessa and decimal to display in floating point */
#define MANTESSA count2/((unsigned long)1000)
#define DECIMAL (count2/((unsigned long)100))%((unsigned long)10)

int main(int argc, char** argv)
{
	char buf[128];
 	unsigned long count1 = 1, count2 = 0; 
	int read_count;
	while(1){		
		printf("******Enter the Sentence******\n");
		// Taking snapshot of OS time before user types.
		count1 = time();
		if ((read_count = read(0, buf, 128)) >= 128){ 
			printf("\n Please stick to line length of 127 \n");
			continue;
		}
		// Taking snapshot of OS time after user types.
		count2 = time();
		// Difference would give total time of typing.
		count2 -= count1;
		printf("You have entered:\n");
		write(STDOUT_FILENO, buf, sizeof(buf)-1);
		// Printing in convoluted way, since %f does not work.
		printf("Typing time is %lu.%lu seconds\n", MANTESSA, DECIMAL);
		printf("------------------------------\n");
	}
	return 0;
}
