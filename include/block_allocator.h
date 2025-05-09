#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include "helpers/memory_aligner.h"
#include "helpers/mmap_helper.h"

#include "types/block.h"

Block* split_chunk(Block** chunk, size_t chunk_size);
Block* combine_two_chunks(Block* chunk_prev, Block* chunk_next);
Block* traverse_and_unlink_block(Block* block, size_t size);
Block* find_free_block(Arena* arena, size_t size);
void* allocate_block(Arena* arena, size_t size);
void free_block(void* ptr);

#endif