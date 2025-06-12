#ifndef TCACHE_H
#define TCACHE_H

#include "types/block.h"

#define TCACHE_MAX_BINS          64

#define TCACHE_BIN_SIZE_STEP 16
#define TCACHE_MAX_SIZE 1024 // 1032
#define TCACHE_NUM_BINS (TCACHE_MAX_SIZE / TCACHE_BIN_SIZE_STEP)

int tcache_find_index(size_t size);
Block* tcache_alloc(int index);
void tcache_remove();

#endif

// (400 + 64 - 1) / 16 - 1