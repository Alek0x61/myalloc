#include "../../include/bins/unsortedbin.h"

void move_to_sorted_bin(Block* block) {
    
}

Block* unsortedbin_try_alloc(Arena* arena, size_t size) {
    Block* current = arena->unsortedbin;
    while (current) {
        if (current->size == size) {
            return pop_block(current);
        }

        if (current->size > size) {
            return split_chunk(&current, size);
        }

        // place it to sorted bins (like fastbin for example)
        Block* next = current->fd;

        Block* tbs_block = pop_block(current);
        tbs_block->bk = tbs_block->fd = NULL;

        move_to_sorted_bin(current);
        current = next;
    }
    return NULL;
}