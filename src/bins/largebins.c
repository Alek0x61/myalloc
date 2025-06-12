#include "../../include/bins/largebins.h"

int largebin_find_index(Arena* arena, size_t size) {
    size_t index = CALCULATE_FAST_BIN_INDEX(size, BLOCK_SIZE);

    if (index >= NUM_BINS || arena->bins[index] == NULL) {
        return -1;
    }

    return index;
}

Block* largebin_alloc(Arena* arena, int index, size_t size) {
    printf("Getting largebin.\n");
    Block* block = traverse_and_unlink_block(arena->bins[index], size);
    block->arena = arena;
    return block;
}

void largebin_insert(Block* new_peer, int index) {
    Block** bin = &new_peer->arena->bins[index];
    Block* curr = *bin;
    Block* prev = NULL;

    while (curr && new_peer->size > curr->size) {
        prev = curr;
        curr = curr->fd;
    }

    new_peer->fd = curr;
    new_peer->bk = prev;

    if (curr)
        curr->bk = new_peer;

    if (prev)
        prev->fd = new_peer;
    else
        *bin = new_peer;
}

void largebin_remove() {
    
}