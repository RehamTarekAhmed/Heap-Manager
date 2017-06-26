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
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAX_HEAP_SIZE (1<<27)
#define ALIGNMENT 	8 /*64-bit systems */
#define ALIGN(x) (((x)+(ALIGNMENT-1)) & ~(ALIGNMENT-1)) /* A macro that rounds up to the nearest multiple of ALIGNMENT */

void* memalloc(unsigned int reqsize); /*Allocates a contiguous block of memory of at least reqsize & returns a ptr of type void*/
void memfree(void *ptr); /* marks the block of memory pointed to by ptr as free*/
bool memalloc_init();
//bool split_block ( block_t* b, unsigned int s);

typedef struct block {/*contains the Meta data for each block, arranged for a better alignment*/
    bool free; // 1 byte
    unsigned int size; /*contains the size of the data object or the amount of free bytes, 2 bytes*/
    block* next; /* 8 bytes each */
    block* prev;
 } block_t;

block_t* fusion (block_t* b);
void print_freelist();
#define METADATA (sizeof(block_t))
#endif // HEAP_H
