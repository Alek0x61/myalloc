#ifndef UNSORTEDBIN_h
#define UNSORTEDBIN_h

#include "../types/block.h"
#include "config.h"
#include "../sys/memory_aligner.h"
#include "bins/largebins.h"
#include "bins/smallbins.h"

#include <stdio.h>

Block* unsortedbin_try_alloc(Arena* arena, size_t size);

#endif


// The unsorted bin is checked first.

// If a chunk is large enough:

// It is split (if needed), and the remaining part might stay in unsorted bin or move to another bin.

// If not suitable:

// The chunk is removed from the unsorted bin and placed into the appropriate bin (small/large).