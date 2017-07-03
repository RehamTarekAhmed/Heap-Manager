/****************************************************************************
* Created: 18/6/2017
* Author: Reham Tarek <RehamTarekAhmed@gmail.com>
* Test1: checking the sainty of the code
* 0-> allocated 1-> free 
*The main list is represented in magenta and the Free list is represented in blue.
 ***************************************************************************/
 #include <assert.h>
 #include "Heap.h"

 int main(int argc, char *argv[])
{
	void *ptr[4];
  int size = MAX_HEAP_SIZE/ 4;

  /* Allocate 3 consecutive chunks 0001*/
  int i;
  for(i=0;i<3;i++)
    {
    ptr[i] = memalloc(size);
    assert(ptr[i]);
    }
  /* Free the middle 0101*/
   memfree(ptr[1]);
   /*allocate a new small chunk, it should be allocated within the smaller free chunk 01001 */
   ptr[3] = memalloc(5000);
   assert(ptr[3]);
   /*Free the 3rd chunk to check the fusion works 0101*/
   memfree(ptr[2]);
   /* Observe both lists */
   print_list();
   /*Free the first chunk 101*/
   memfree(ptr[0]);
   print_list();
   /*1*/
   memfree(ptr[3]);
   print_list();
  return 0;
}
