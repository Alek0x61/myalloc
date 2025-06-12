#include "../../include/bins/fastbins.h"

int fastbin_find_index(Arena* arena, size_t size) {
    size_t index = CALCULATE_FAST_BIN_INDEX(size, BLOCK_SIZE);

    if (index >= NUM_FASTBINS || arena->fastbins[index] == NULL) {
        return -1;
    }

    return index;
}

Block* fastbin_alloc(Arena* arena, int index) {
    printf("Getting fastbin.\n");
    Block* block = arena->fastbins[index];

    arena->fastbins[index] = block->fd;
    block->arena = arena;
    return block;
}

void fastbin_remove() {
    
}