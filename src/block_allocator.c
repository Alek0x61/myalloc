#include "../include/block_allocator.h"

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

Block* combine_two_chunks(Block* chunk_prev, Block* chunk_next) { //directly combining chunks to avoid fragmentation
    chunk_prev->size += chunk_next->size;
    chunk_next->size=0;
    return chunk_prev;
}

Block* traverse_and_unlink_block(Block* block, size_t size) {
    Block* curr = block;

    while (curr) {
        if (curr->size >= size) {
            if (curr->bk) {
                curr->bk->fd = curr->fd;
            }
            if (curr->fd) {
                curr->fd->bk = curr->bk;
            }

            return curr;
        }
        curr = curr->fd;
    }

    return NULL;
}

Block* find_free_block(Arena* arena, size_t size) {
    Block* block = NULL;

    if (size >= M_MMAP_THRESHOLD) {
        size_t metadata_size = ENCODE_AS_MMAPPED(size);
        block = request_block_memory(metadata_size, size);
        return block;
    }

    size_t index = CALCULATE_FAST_BIN_INDEX(size, BLOCK_SIZE);

    // TODO should look at tcache first, but only for exact size match
    // fastbin lookup
    if (index < NUM_FASTBINS && arena->fastbins[index]) {
        printf("Found fastbin\n");
        block = arena->fastbins[index];
        arena->fastbins[size] = block->fd; 
        block->arena = arena;
        return block;
    }

    index = CALCULATE_SMALL_BIN_INDEX(size, BLOCK_SIZE);
    //printf("index: %d\n", index);
    // smallbin lookup if its under the size threshold
    if (index < SMALLBIN_THRESHOLD && arena->bins[index]) {
        printf("Found smallbin\n");
        block = arena->bins[index];
        arena->bins[size] = block->fd;
        block->arena = arena;
        return block;
    }

    index = CALCULATE_LARGE_BIN_INDEX(size);

    if (index < NUM_BINS) {
        Block* large_block = traverse_and_unlink_block(arena->bins[index], size);
        if (large_block) {
            printf("Found largebin\n");
            return large_block;
        }
    }

    // looking up unsorted
    // Block* unsorted_block = traverse_bin_blocks(arena->unsortedbin, size);

    // if (unsorted_block) {
    //     return unsorted_block;
    // }
    
    //TODO sort unsorted blocks into small and large bins
    if (!block) {
        if (arena->top_chunk->size < size) {
            Block* top_chunk_prev = arena->top_chunk;
            arena->top_chunk = request_top_chunk_memory();
            arena->top_chunk = combine_two_chunks(top_chunk_prev, arena->top_chunk);
        }
        block = split_chunk(&arena->top_chunk, size);
        printf("Splitting from top chunk\n");
        return block;   
    }

    return block;   
}

void* allocate_block(Arena* arena, size_t size) {
    Block* block;

    size += BLOCK_SIZE;
    size = ALIGN_SIZE(size);

    block = find_free_block(arena, size);

    printf("block: %p\n", block);

    return (void*)(block + 1);
}

void free_block(void* ptr) {
    if (!ptr)
        return;
    Block* block = (Block*)ptr - 1; //-1 to get the metadata

    if (block->size == 0) // Sanity check
        return;

    Arena* arena = block->arena;

    if (IS_MMAPPED(block->size)) {
        size_t decoded_size = DECODE_MMAPPED_SIZE(block->size);
        int ret = munmap(block, decoded_size);
        printf("munmap result: %d\n", ret);
    }


    pthread_mutex_lock(&arena->lock);

    size_t index = CALCULATE_FAST_BIN_INDEX(block->size, BLOCK_SIZE);
    if (index < NUM_FASTBINS) {
        block->fd = arena->fastbins[index];
        arena->fastbins[index] = block;
        return;
    }
    

    index = CALCULATE_SMALL_BIN_INDEX(block->size, BLOCK_SIZE);

    printf("free-block-index: %d\n", index);
    printf("free-block-size: %d\n", block->size);
    pthread_mutex_unlock(&arena->lock);
}