#ifndef LARGEBINS_H
#define LARGEBINS_H

#include "helpers/block_helper.h"

#include "types/block.h"
#include "config.h"
#include "sys/memory_aligner.h"

#include <stdio.h>

int largebin_find_index(Arena* arena, size_t size);
Block* largebin_alloc(Arena* arena, int index, size_t size);
void largebin_insert(Block* new_peer, int index);
void largebin_remove();

#endif