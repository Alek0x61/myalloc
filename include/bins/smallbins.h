#ifndef SMALLBINS_H
#define SMALLBINS_H

#include "../types/block.h"
#include "config.h"
#include "../sys/memory_aligner.h"

#include <stdio.h>

int smallbin_find_index(Arena* arena, size_t size);
Block* smallbin_alloc(Arena* arena, int index);
void smallbin_remove();

#endif