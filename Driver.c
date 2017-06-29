/****************************************************************************
* Version: 1.0
* Created: 18/6/2017
* Author: Reham Tarek <RehamTarekAhmed@gmail.com>
* Driver
 ***************************************************************************/
 #include "Heap.h"

 int main(int argc, char *argv[])
{
	int size;
	void *ptr[10];
	size = MAX_HEAP_SIZE/ 4;

	 ptr[0] = memalloc(size);
	 ptr[1] = memalloc(size);
	 ptr[2] = memalloc(size);
   ptr[4] = memalloc(5);
   memfree(ptr[0]);
   ptr[5] = memalloc(5555);
   memfree(ptr[2]);
   memfree(ptr[4]);
   ptr[6] = memalloc(8720);
   print_list();
   printf("\n");
   //ptr[7] = memalloc(33539976);

   memfree(ptr[5]);
   memfree(ptr[6]);
   print_list();
   printf("\n");

	exit(0);
}
