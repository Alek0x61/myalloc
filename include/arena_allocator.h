#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include "block_allocator.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>
#include <pthread.h>

extern Arena** arenas;
extern int num_arenas;
extern int max_arenas;

void init_bins(Arena* arena);
Arena* create_new_arena();
int get_max_arenas();
void init_arenas(void);
Arena* get_arena();
void* new(size_t size);
void free(void* ptr);

#endif