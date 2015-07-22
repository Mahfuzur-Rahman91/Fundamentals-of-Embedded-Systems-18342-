@ exit.S: exit sycall wrapper
@
@ Authors: Sudhir Kumar Vijay <svijay@andrew.cmu.edu> 
@ Date:    5th October, 2014 6:58PM 

#include <bits/swi.h>
	.file	"exit.S"
	.text

	.global	exit
exit:
	swi	EXIT_SWI @Calling EXIT_SWI.
