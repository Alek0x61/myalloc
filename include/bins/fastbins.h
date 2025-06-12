#ifndef FASTBINS_H
#define FASTBINS_H

#include "../types/block.h"
#include "config.h"
#include "../sys/memory_aligner.h"

#include <stdio.h>

int fastbin_find_index(Arena* arena, size_t size);
Block* fastbin_alloc(Arena* arena, int index);
void fastbin_remove();

#endif