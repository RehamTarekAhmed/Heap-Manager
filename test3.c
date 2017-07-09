/****************************************************************************
* Version: 1.0
* Created: 18/6/2017
* Author: Reham Tarek <RehamTarekAhmed@gmail.com>
* Test3: Obtain the avg time for both functions
 ***************************************************************************/
 //#include <time.h>
 #include <assert.h>
 //#include <sys/resource.h>
 #include <sys/time.h>
 #include "Heap.h"
 #define RAND() (drand48())
 #define SEED(x) (srand48((x)))
 #define PSEUDO_RANDOM_SEED 0 /*toggle it for random results for every run*/
 #define LOOPCNT 50000
 #define MAX_ALLOC_SIZE (MAX_HEAP_SIZE/10000) /**/
 //#undef calculate
 //#undef getrusage


 double time_diff(struct timeval x , struct timeval y);
 //double calculate(const struct rusage* b, const struct rusage* a);

 int main(int argc, char *argv[])
{     struct timeval begin, end, begin2, end2;
  int size;
  int itr;
  void *ptr[LOOPCNT]={NULL};
  int i,j=0;
  double randvar;
  int fail = 0, success=0;
  //clock_t begin, end;
  double time_spent=0.0,time_spent2=0.0;

  /* Set the PSEUDO_RANDOM_SEED for pseduo random seed initialization based on time */
  if(PSEUDO_RANDOM_SEED)
    SEED(time(NULL));
  //begin = clock();
  for(i = 0; i < LOOPCNT; i++)
  {
    itr = (int)(RAND() * LOOPCNT);
    randvar = RAND();

      size = (int)(RAND() * MAX_ALLOC_SIZE);
      while(size == 0)
          size = (int)(RAND() * MAX_ALLOC_SIZE);
        gettimeofday(&begin , NULL);
        ptr[i] = memalloc(size);
				gettimeofday(&end , NULL);

    if(ptr[i] == NULL) {
      //printf("malloc at iteration %d failed for size %d\n", i,size);
      ++fail;
    }
    else
    {    ++success;
         time_spent+= time_diff(begin, end);
    }

    if( ptr[itr] != NULL) {
      //printf("Freeing ptr[%d]\n", itr);
      gettimeofday(&begin2 , NULL);
      memfree(ptr[itr]);
			gettimeofday(&end2 , NULL);
			time_spent2 += time_diff(begin2, end2);
			j++;
            ptr[itr] = NULL;
    }
  }

//print_list();
  /*
  * now -- free them
  * */
  for(i=0; i < LOOPCNT; i++) {
    if(ptr[i] != NULL) {
      gettimeofday(&begin2 , NULL);
      memfree(ptr[i]);
      gettimeofday(&end2 , NULL);
      time_spent2 += time_diff(begin2, end2);
      j++;
      ptr[i] = NULL;
    }
  }
  //end = clock();

//  print_list();
  printf("\n");
  //time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Test case summary\n");
  printf("Loop count: %d, malloc successful: %d, malloc failed: %d\n", success+fail, success, fail);
  printf("Average memalloc execution time: %g microseconds\t Average memfree execution time:%g microseconds\n", time_spent/success,time_spent2/j);
  return 0;
}

double time_diff(struct timeval x , struct timeval y)
{
    double diff = (y.tv_sec - x.tv_sec)*1000000 + (y.tv_usec - x.tv_usec);
    return diff;
}
/*double calculate(const struct rusage* b, const struct rusage* a)
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
*/
