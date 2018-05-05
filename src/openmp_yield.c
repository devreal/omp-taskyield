#include <omp.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NUM_TASKS 1000

#define VERBOSE


int main(int argc, char **argv)
{
  volatile int flag_one_cntr = 0;
  volatile int flag_two_cntr = 0;
#pragma omp parallel
#pragma omp master
  for (int i = 0; i < NUM_TASKS+omp_get_num_threads()-1; ++i) {
#pragma omp task firstprivate(i)
{
    if (omp_get_thread_num() > 0) {
      // trap all but thread 0
      printf("Trapping thread %d\n", omp_get_thread_num());
      while(flag_two_cntr != NUM_TASKS) { }
      printf("Un-Trapping thread %d\n", omp_get_thread_num());
    } else {
      int task_id = ++flag_one_cntr;
#pragma omp taskyield
      // when we come back we first check the counter
      if (task_id == 1) {
        if (task_id == flag_one_cntr) {
          printf("NOOP\n");
        } 
        // some other tasks were running in between
        else if (flag_two_cntr == (NUM_TASKS - 1)) {
          printf("STACK (unlimited)\n");
        } else if (flag_two_cntr == flag_one_cntr-1) {
          printf("STACK(depth=%d)\n", flag_one_cntr);
        } else if (flag_one_cntr == (NUM_TASKS) /*&& flag_two_cntr == 0*/) {
          printf("CYCLIC\n");
        } else if (flag_one_cntr > 0 /*&& flag_two_cntr == 0*/) {
          printf("N-CYCLIC (N=%d)\n", flag_one_cntr);
        } else {
          printf("UNKNOWN: flag_one_cntr: %d, flag_two_cntr: %d\n", flag_one_cntr, flag_two_cntr);
        }
      }
      (void)flag_two_cntr;
#pragma omp taskyield
      ++flag_two_cntr;
    } // thread-trap
} // pragma omp task
  } // for()
  return 0;
}

