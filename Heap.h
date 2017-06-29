/****************************************************************************
 * Heap.h
 * Declares the Heap Manager's functionality.
 * Author: Reham Tarek <RehamTarekAhmed@gmail.com>
 ***************************************************************************/
#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE (1<<27) /*134217728*/
#define ALIGNMENT 	8 /*64-bit systems */
#define Hashed      4
#define ALIGN(x) (((x)+(ALIGNMENT-1)) & ~(ALIGNMENT-1)) /* A macro that rounds up to the nearest multiple of ALIGNMENT */

typedef struct block {/*contains the Metadata for each block, arranged for a better alignment*/
    bool free; /* 1 byte*/
    bool end; /*like our old fashoned NULL indicates the end of the list*/
    char freedirection;
    unsigned int size; /*contains the size of the data object or the amount of free bytes, 4 bytes*/
    unsigned int prevsize; /*zero means no previous block*/
    unsigned int prevfree;
    struct block* nextf; //or else use padding
 } block_t;

#define METADATA (sizeof(block_t))

void* memalloc(unsigned int reqsize); /*Allocates a contiguous block of memory of at least reqsize & returns a ptr of type void*/
void memfree(void *ptr); /* marks the block of memory pointed to by ptr as free*/
bool memalloc_init();
//bool split_block ( block_t* b, unsigned int s);
bool insertf(block_t* freeblock);
bool removef(block_t* freeblock);

block_t* fusion (block_t* b);
void print_list(); /*DEBUGGING*/
int hash_function( unsigned int size);
void set_dir(block_t* p,block_t* n); /*2nd-1st */
block_t* set_prevf(block_t* p);
block_t* set_Next(block_t* p);
block_t* set_prev(block_t* p);

#endif
