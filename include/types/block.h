#include <stdlib.h>

typedef struct Block {
    size_t prev_size;
    size_t size;
    struct Block* fd;
    struct Block* bk;
    struct Block* fd_nextsize;
    struct Block* bk_nextsize;
    int free;
} Block;
