/****************************************************************************
 * Heap.c
 * The implementation of the Heap Manager's functionality.
  * Author: Reham Tarek <RehamTarekAhmed@gmail.com>
 ***************************************************************************/
#include "Heap.h"

block_t* head = NULL;

bool memalloc_init()
{
  head = (block_t*)malloc(MAX_HEAP_SIZE); /*sbrk could have been used but I sticked to the pdf*/
  if(!head) return false;
  head->next = NULL;
  head->prev = NULL;
  head->size = MAX_HEAP_SIZE-METADATA;
  head->free = true;
  return true;
}

void* memalloc(unsigned int reqsize)
{
  reqsize= ALIGN(reqsize);
  if(head == NULL)
    if( (!memalloc_init()) || reqsize <= 0 ) return NULL;
    block_t* current = head; /*Start from the first block*/
    //block_t* pred=current;
    while ((current!=NULL) && !(current->free && current->size >= reqsize)) /*loop over blocks*/
      {  //pred=current;
         current = current->next;
       }
    if (current==NULL)
         return NULL;
    current->free = false; /* we found a block \o/ */
    if (current->size >= (reqsize+METADATA+8)) /*if the rest after spiltting supports the metadata and a minimal block*/
    {
      block_t* newblock;
      newblock->free = true;
      newblock->size = current->size-reqsize-METADATA;
      newblock->next = current->next;
      newblock->prev = current;
      current->next = newblock;
      if(newblock->next)
          newblock->next->prev = newblock;
    } /*then split*/
    current->size = reqsize;
    return (((void*)current) + METADATA); /* return a pointer to the allocated segment */
}

void memfree(void *ptr)
{
  block_t* f = ((block_t*) ptr) - METADATA;
  if (f== NULL) /*Return if invalid block*/
    return;
  f->free = true;
  if(f->prev && f->prev->free)
    f = fusion (f->prev);
  if (f->next)
    fusion (f);
 else /*we are at the end of the allocated memory */
  { if (f->prev)
    f->prev ->next = NULL;
    else
    /* No more block !*/
    head = NULL;
  }
}
  block_t* fusion (block_t* b)
  {
    if (b->next && b->next ->free )
    {
      b->size += METADATA + b->next ->size;
      b->next = b->next ->next;
      if (b->next)
      b->next->prev = b;
    }
  return (b);
  }
