#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include "helpers/block_helper.h"

#include "bins/fastbins.h"
#include "bins/largebins.h"
#include "bins/smallbins.h"
#include "bins/tcache.h"

#include "types/bin_type.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stddef.h>

#include "sys/memory_aligner.h"
#include "sys/mmap_helper.h"

#include "types/block.h"
#include "arena_lock.h"

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