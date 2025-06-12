#include "../../include/helpers/block_helper.h"

Block* pop_block(Block* block) {
    if (block->bk) {
        block->bk->fd = block->fd;
    }
    if (block->fd) {
        block->fd->bk = block->bk;
    }

    return block; 
}

Block* traverse_and_unlink_block(Block* block, size_t size) {
    Block* curr = block;

    while (curr) {
        if (curr->size >= size) {
            return pop_block(curr);
        }
        curr = curr->fd;
    }

    return NULL;
}

Block* combine_two_chunks(Block* chunk_prev, Block* chunk_next) {
    chunk_prev->size += chunk_next->size;
    chunk_next->size=0;
    return chunk_prev;
}

Block* split_chunk(Block** chunk_ptr, size_t chunk_size) {
    // TODO need to add check if the chunk_ptr has available space at all
    Block* original = *chunk_ptr;
    size_t full_size = original->size;

    if (full_size < chunk_size) { 
        if (full_size > 0) { // do something with the original block (put it in unsorted)
        
        }
            
        original = request_top_chunk_memory();
        full_size = original->size;
    }

    Block* new_block = original;
    new_block->size = chunk_size;

    Block* remainder_block = (Block*)((char*)original + chunk_size);
    remainder_block->size = full_size - chunk_size;

    *chunk_ptr = remainder_block;

    return new_block;
}