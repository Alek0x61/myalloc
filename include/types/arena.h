#ifndef ARENA_H
#define ARENA_H

#include <pthread.h>

#define NUM_FASTBINS             14
#define SMALLBIN_THRESHOLD       16
#define NUM_BINS                 80

struct Block;

typedef struct Arena {
    pthread_mutex_t lock;
    struct Block* blockList;
    
    struct Block* fastbins[NUM_FASTBINS];
    struct Block* bins[NUM_BINS];
    struct Block* unsortedbin;
    
    struct Block* top_chunk;
} Arena;

#endif