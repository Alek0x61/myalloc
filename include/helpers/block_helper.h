#ifndef BLOCK_HELPER_H
#define BLOCK_HELPER_H

#include "../types/block.h"
#include "sys/mmap_helper.h"

Block* pop_block(Block* block);
Block* traverse_and_unlink_block(Block* block, size_t size);
Block* combine_two_chunks(Block* chunk_prev, Block* chunk_next);
Block* split_chunk(Block** chunk_ptr, size_t chunk_size);

#endif