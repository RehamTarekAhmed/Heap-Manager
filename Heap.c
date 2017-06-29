/****************************************************************************
 * Heap.c
 * The implementation of the Heap Manager's functionality.
 * Author: Reham Tarek <RehamTarekAhmed@gmail.com>
 ***************************************************************************/
#include "Heap.h"

block_t* head = NULL;
block_t* headf = NULL;
block_t* freearray[Hashed]={NULL}; /*to improve the worst case scenario by a factor of Hashed XXX*/

/*-----------------------------------------------------------------------------------------------------------*/
int hash_function( unsigned int size)
 {
   int x= size / (MAX_HEAP_SIZE/Hashed);
   return x;
 }
 /*-----------------------------------------------------------------------------------------------------------*/

bool memalloc_init()
{
  head = (block_t*)malloc(MAX_HEAP_SIZE); /*sbrk could have been used but I sticked to the pdf*/
  if(!head) return false;
  head->end = true;
  head->size = MAX_HEAP_SIZE-METADATA;
  head->free = true;
  head->prevsize=0;
  head->nextf=NULL;
  head->prevfree=0;
  head->freedirection= '+';
  freearray[Hashed]=head;
  insertf(head);
  return true;
}
/*-----------------------------------------------------------------------------------------------------------*/
void* memalloc(unsigned int reqsize)
{
    reqsize= ALIGN(reqsize);
    if(reqsize <= 0 || reqsize > MAX_HEAP_SIZE-METADATA || ((head == NULL) && !memalloc_init()))
          return NULL;
    int x=hash_function(reqsize);     /* A range to start the search*/
    while((!freearray[x])&& x < Hashed-1) /* If you encountered a NULL, keep looking for other bigger free chunks*/
            x++;
      block_t* current = freearray[x]; /* Either a NULL or a head */
    while ((current!=NULL) && !(current->free && reqsize< current->size < reqsize)) /*Loop over free blocks starting with the head*/
         current = current->nextf;
    if (current==NULL)
         return NULL; /*No suitable chunk was found*/
    removef(current); /* Remove the chunk you found from the free list */
    current->free = false;
    if (current->size >= (reqsize+METADATA+ALIGNMENT)) /*Split if the chunk after spiltting supports the metadata and a minimal block*/
    {
      block_t* newblock=(block_t *)(((char*) current) + reqsize + METADATA);
      newblock->free = true;
      newblock->size = current->size-reqsize-METADATA;
      newblock->prevsize= reqsize;
      newblock->end=current->end;
      newblock->prevfree=0;
      newblock->freedirection='+';
      newblock->nextf = NULL;
      current->size = reqsize;
      current->end=false;
      block_t* next=set_Next(newblock);
      if(next)
          next->prevsize=newblock->size;
      insertf(newblock);     /*inserting the new block in the free list*/
  } /*split complete*/

    return (void*)(((char*)current) + METADATA); /* return a pointer to the allocated segment */
}

/*-----------------------------------------------------------------------------------------------------------*/
void memfree(void *ptr)
{
  if (ptr== NULL) /*Return if invalid block*/
    return;
  block_t* to_free = (block_t*)((char*)ptr - METADATA);
  block_t* pre =set_prev(to_free); /*Obtain the previous block*/
  to_free->free = true;
  if(pre && pre->free)
  {
    removef(pre);
    to_free = fusion (pre);
  }
  block_t* next= set_Next(to_free); /*Obtain the next block*/
  if(next && next->free)
  {
    removef(next);
    fusion (to_free);
  }
  insertf(to_free);
}
/*-----------------------------------------------------------------------------------------------------------*/
block_t* fusion (block_t* b)
  {
    block_t* next= set_Next(b);
    b->size += METADATA + next->size;
    b->end=next->end;
    if (!b->end)
      {next=set_Next(b);
      next->prevsize= b->size;}
  return (b);
  }
  /*-----------------------------------------------------------------------------------------------------------*/
  bool removef(block_t* freeblock)
  {  if (!freeblock->free)
          return false;
    block_t* prev= set_prevf(freeblock);
    int x=hash_function(freeblock->size);
    if(prev) /*remove it from the list */
      {
        prev->nextf=freeblock->nextf;
        if (freeblock->nextf)
            set_dir(freeblock->nextf, prev);
        if (freearray[x]==freeblock && freeblock->nextf &&(hash_function(freeblock->nextf->size)==x))
            freearray[x]=freeblock->nextf;
        else if(freearray[x]==freeblock)
              freearray[x]=NULL;

      }
      else /* there was one element or the starting element*/
      {    if(freeblock==headf)
              headf=headf->nextf;
        if(freeblock->nextf && (hash_function(freeblock->nextf->size)==x))
              freearray[x]=freeblock->nextf;
        else
            freearray[x]=NULL;
        if(freeblock->nextf)
            freeblock->nextf->prevfree=0;
      //  headf=freearray[x];
      }
      freeblock->nextf=NULL;
      freeblock->prevfree=0;
      return true;
  }
/*-----------------------------------------------------------------------------------------------------------*/
bool insertf(block_t* freeblock)
{ block_t* pred;
  int x=hash_function(freeblock->size);
  pred=freearray[x];
  block_t* n=NULL; int y=x;
  if (pred == NULL) /*Means either at head or the first in the sector*/
      { freearray[x] = freeblock;
        while(y<(Hashed-1) && !n)
          { y++;
            n=freearray[y];
          }
         if(!n) /*No next, find the pred*/
         {
           y=x;
           while(y>0 && !pred)
            { y--;
              pred=freearray[y];
            }
           while(pred && pred->nextf && hash_function(pred->nextf->size) < x)
              pred=pred->nextf;
           freeblock->nextf=NULL;
           }
         else /*If there is a next get the prev directly (can be NULL)*/
          {
           pred=set_prevf(n);
           freeblock->nextf=n;
           set_dir(n,freeblock);
          }
         if (pred)
         {
           set_dir(freeblock, pred);
           pred->nextf=freeblock;
          }
         else
         {
           headf=freeblock;
           freeblock->prevfree=0;
          }
      }
  else if(pred && freeblock->size <= pred->size) /*insert at head*/
      {  n=set_prevf(pred); /* Just reusing the declared var NOTE: Refers to the prev*/
         if(n)
         {set_dir(freeblock,n);
         n->nextf=freeblock;}
         else
          headf=freeblock;
         set_dir(pred, freeblock);
         freeblock->nextf = pred;
         freearray[x] = freeblock;
      }
  else
      {
          while (pred->nextf != NULL && (pred->nextf->size < freeblock->size))
                pred = pred->nextf;
          freeblock->nextf = pred->nextf;
          pred->nextf = freeblock;
          if(freeblock->nextf)
            set_dir(freeblock->nextf,freeblock);
          set_dir(freeblock, pred);
        }
}
/*-----------------------------------------------------------------------------------------------------------*/
void set_dir(block_t* p,block_t* n)
{ if (!p || !n)
      return;
  p->prevfree= abs((char*)p-(char*)n); /*TODO try them without casting */
  if((p-n)>0)
      p->freedirection='+';
  else
      p->freedirection='-';
}
/*-----------------------------------------------------------------------------------------------------------*/
block_t* set_prevf(block_t* p)
{ if (!p || p->prevfree==0 ) return NULL;
  if (p->freedirection=='-')
    return ((block_t*)((char*)p + p->prevfree));
  else
    return ((block_t*)((char*)p - p->prevfree));
}

block_t* set_prev(block_t* p)
{
 if(p && p->prevsize!=0)
    return (block_t*)((char*)p - METADATA- p->prevsize);
  else
    return NULL;
}

block_t* set_Next(block_t* p)
{
   if (p && !(p->end))
    return (block_t*)((char*)p + METADATA+ p->size);
  else
    return NULL;
}
/*-----------------------------------------------------------------------------------------------------------*/
void print_list()
{
      block_t *blocklist_head = head;
      while(blocklist_head) {
         block_t* next= set_Next(blocklist_head);
         block_t* prev= set_prev(blocklist_head);
         printf( "\x1b[35m \t blocklist Size:%u, Head:%p, Prev:%p, Next:%p, Free:%d\t\n \x1b[0m",blocklist_head->size,blocklist_head,prev,next,blocklist_head->free);
         blocklist_head = next;
      }
      blocklist_head = headf;
      while(blocklist_head){
         block_t* prevf= set_prevf(blocklist_head);
         printf( "\x1b[34m \t blocklist Size:%u, Head:%p, Prev Free:%p, Next Free:%p, Free:%d\t\n \x1b[0m",blocklist_head->size,blocklist_head,prevf,blocklist_head->nextf,blocklist_head->free);
         blocklist_head = blocklist_head->nextf;
      }
      printf("\n");
  }
