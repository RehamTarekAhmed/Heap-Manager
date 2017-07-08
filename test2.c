/****************************************************************************
* Version: 1.0
* Created: 18/6/2017
* Author: Reham Tarek <RehamTarekAhmed@gmail.com>
* Test2: Checking efficiency
 ***************************************************************************/
 //#include <time.h>
 #include <assert.h>
 #include <sys/resource.h>
 #include <sys/time.h>
 #include "Heap.h"
 #define RAND() (drand48())
 #define SEED(x) (srand48((x)))
 #define PSEUDO_RANDOM_SEED 0 /*toggle it for random results for every run*/
 #define LOOPCNT 50000
 #define MAX_ALLOC_SIZE (MAX_HEAP_SIZE/1000) /**/
 #undef calculate
 #undef getrusage

 double calculate(const struct rusage* b, const struct rusage* a);

 int main(int argc, char *argv[])
{     struct rusage begin, end;
  int size;
  int itr;
  void *ptr[LOOPCNT]={NULL};
  int i,j;
  double randvar;
  int fail = 0, success=0;
  //clock_t begin, end;
  double time_spent;

  /* Set the PSEUDO_RANDOM_SEED for pseduo random seed initialization based on time */
  if(PSEUDO_RANDOM_SEED)
    SEED(time(NULL));

  //begin = clock();
  getrusage(RUSAGE_SELF, &begin);
  for(i = 0; i < LOOPCNT; i++)
  {
    itr = (int)(RAND() * LOOPCNT);
    randvar = RAND();

      size = (int)(RAND() * MAX_ALLOC_SIZE);
      while(size == 0)
          size = (int)(RAND() * MAX_ALLOC_SIZE);
        ptr[i] = memalloc(size);


    if(ptr[i] == NULL) {
      //printf("malloc at iteration %d failed for size %d\n", i,size);
      ++fail;
    }
    else
    ++success;
		


    if( ptr[itr] != NULL) {
    //  printf("Freeing ptr[%d]\n", itr);
      memfree(ptr[itr]);
      ptr[itr] = NULL;
    }
  }
  getrusage(RUSAGE_SELF, &end);
//print_list();
  /*
  * now -- free them
  * */
  //print_list();
  for(i=0; i < LOOPCNT; i++) {
    if(ptr[i] != NULL) {
      memfree(ptr[i]);
      ptr[i] = NULL;
    }
  }
  //end = clock();
  time_spent= calculate(&begin, &end);
  print_list();
  printf("\n");
  //time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Test case summary\n");
  printf("Loop count: %d, malloc successful: %d, malloc failed: %d, execution time: %g seconds\n\n", success+fail, success, fail, time_spent);

  return 0;
}

double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec))+
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))/ 1000000.0);
    }
}
