#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include "arena.h"

typedef struct Block {
    size_t size;
    struct Block* fd;
    struct Block* bk;
    Arena* arena;
} Block;

#endif