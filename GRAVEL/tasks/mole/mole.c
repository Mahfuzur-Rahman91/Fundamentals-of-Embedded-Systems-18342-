/** @file mole.c
 * Authors: Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 * 			Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @brief: 	Displays a mole game in which moles (denoted by 'O')
 *			pop up and are hit (denoted by 'x').
 * Links to libc.
 */

#include<stdio.h>
#include<unistd.h>

void print_mole(char mole_element[9]);

int main(int argc, char** argv)
{
	int sum    = 0;
	int seed_1 = 1;
	int seed_2 = 2;
	int rand_num = 0;

	char mole_element[] = {'O','O','O','O','O','O','O','O'};
	int i = 0;
	printf("***************************************\n");
	printf("*** You just invoked the Mole Game  ***\n");
	printf("*** 	So much of endless fun !    ***\n");
	printf("---------------------------------------\n");
	printf("*** 	' ': Indicates no mole      ***\n");
	printf("*** 	'O': Indicates mole         ***\n");
	printf("*** 	'X': Indicates popped mole  ***\n");
	printf("***************************************\n");

	while(1){	
		/* Fibonacci + mod based pseudo-random number generator */
		sum = seed_1 + seed_2;
		seed_1 = seed_2;
		seed_2 = sum;
		rand_num = (sum%9);        

		for (i = 0; i<9; i++)
			mole_element[i] = ' ';

		print_mole(mole_element);
		printf("No moles              \n");

		sleep(1000);
		printf("\033[8A"); // Move cursor up 8 rows.
		printf("\r"); // Carriage return on first row.
		mole_element[rand_num] = 'O';

		print_mole(mole_element);
		printf("A sneaky mole appears \n");

		sleep(1000);
		printf("\033[8A"); 
		printf("\r");
		mole_element[rand_num] = 'X';

		print_mole(mole_element);
		printf("BAM! Mole popped !   \n");

		sleep(1000);
		printf("\033[8A");
		printf("\r");
	}
	
	return 0;
}

/* Function to print mole locations  and values in correct format */
void print_mole(char mole_element[9]){
	printf("-------------\n");
	printf("| %c | %c | %c ]\n", mole_element[0], mole_element[1], mole_element[2]);
	printf("-------------\n");
	printf("| %c | %c | %c ]\n", mole_element[3], mole_element[4], mole_element[5]);
	printf("-------------\n");
	printf("| %c | %c | %c ]\n", mole_element[6], mole_element[7], mole_element[8]);
	printf("-------------\n");
	return;
}
