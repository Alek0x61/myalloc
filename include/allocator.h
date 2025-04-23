#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "types/arena.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>
#include <pthread.h>

extern Arena** arenas;
extern int num_arenas;
extern int max_arenas;

// #define SBRK_LIMIT  (128 * 1024) //128KB 32bit
#define LIMIT_32MB  (32 * 1024 * 1024) //32mb 64bit
#define BLOCK_SIZE  sizeof(Block)

void init_arenas(void);
void* new(size_t size);

#endif