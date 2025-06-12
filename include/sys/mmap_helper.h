#ifndef MMMAP_HELPER_H
#define MMMAP_HELPER_H

#include "../types/block.h"
#include "config.h"

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

size_t mmap_page_align(size_t size);
Block* request_top_chunk_memory();
Block* request_block_memory(size_t metadata_size, size_t mmapped_size);

#endif