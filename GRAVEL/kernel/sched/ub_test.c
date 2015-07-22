/** @file ub_test.c
 *
 * @brief The UB Test for basic schedulability
 * @author Sudhir Kumar Vijay <svijay@andrew.cmu.edu>
 *         Sumanth Suresh <sumanths@andrew.cmu.edu>
 * @date 2014-11-25
 */

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

#include <ub.h>
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks , size_t num_tasks)
{
	int i, k ;
	task_t* tasksp = (task_t*)*tasks;
	unsigned long term1, lhs, rhs;

    /* UB test */
    for (k=0; k<(int) num_tasks; ++k) {
    	term1 = 0;
    	for (i=0; i<(int) k-1; ++i) {
    		term1 += ((tasksp[i].C*1000)/tasksp[i].T) ;
    	}
    	/* UB Test LHS */
    	lhs = (term1 + (((tasksp[k].C + tasksp[k].B)*1000)/tasksp[k].T));
		/* UB Test RHS */
		rhs = ub_factor[k];
		if (lhs > rhs){
			/* If UB Test fails for tasks, return back */
			return 0;
		}
	}	
	return 1;
}
