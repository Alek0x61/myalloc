#include "../../include/bins/smallbins.h"

int smallbin_find_index(Arena* arena, size_t size) {
    size_t index = CALCULATE_SMALL_BIN_INDEX(size, BLOCK_SIZE);

    if (index >= SMALLBIN_THRESHOLD || arena->bins[index] == NULL) {
        return -1;
    }

    return index;
}

Block* smallbin_alloc(Arena* arena, int index) {
    printf("Getting smallbin.\n");
    Block* block = arena->bins[index];

    arena->bins[index] = block->fd;
    block->arena = arena;
    return block;
}

void smallbin_insert(Block* new_head, int index) {
    Block* head = new_head->arena->bins[index];
    
    if (head != NULL) {
        new_head->fd = head;
    }

    head->arena->bins[index] = new_head;
}


void smallbin_remove() {
    
}