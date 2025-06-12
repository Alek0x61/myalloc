//Per thread fastbin with depth limit

#include "../../include/bins/tcache.h"

static __thread Block* tcache[TCACHE_MAX_BINS];

int tcache_find_index(size_t size) {
    int index = (size - TCACHE_BIN_SIZE_STEP) / TCACHE_BIN_SIZE_STEP;
    if (index >= TCACHE_MAX_BINS || tcache[index] == NULL) {
        return -1;
    }

    return index;
}

Block* tcache_alloc(int index) {
    Block* block = tcache[index];

    tcache[index] = block->fd;
    return block;
}

void tcache_remove() {

}
