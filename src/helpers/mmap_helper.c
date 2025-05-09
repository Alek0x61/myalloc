#include "../../include/helpers/mmap_helper.h"

static size_t page_size;

size_t mmap_page_align(size_t size) {
    return ((size + page_size - 1) / page_size) * page_size;
}

Block* request_top_chunk_memory() {
    if (page_size == 0) {
        page_size = getpagesize();
    }

    size_t size = mmap_page_align(INITIAL_TOP_SIZE + BLOCK_SIZE);
    return request_block_memory(size, size);
}

Block* request_block_memory(size_t metadata_size, size_t mmapped_size) {  
    Block* block = (Block*) mmap(NULL, mmapped_size, 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);

    if (block == MAP_FAILED)
        return NULL;

    block->size = metadata_size;

    printf("request_block_memory.block %p\n", block);

    return block;
}