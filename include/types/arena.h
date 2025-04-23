#include <pthread.h>
#include "block.h"

typedef struct Arena {
    pthread_mutex_t lock;
    Block* blockList;
    //struct Arena* next;
} Arena;