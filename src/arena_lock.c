#include "arena_lock.h"

void init_arena_lock(Arena* arena) {
    pthread_mutex_init(&arena->lock, NULL);
}

void lock_arena(Arena* arena) {
    pthread_mutex_lock(&arena->lock);
}

void unlock_arena(Arena* arena) {
    pthread_mutex_unlock(&arena->lock);
}

static pthread_mutex_t global_arena_lock = PTHREAD_MUTEX_INITIALIZER;

void init_global_lock() {
    pthread_mutex_init(&global_arena_lock, NULL);
}

void global_lock() {
    pthread_mutex_lock(&global_arena_lock);
}

void global_unlock() {
    pthread_mutex_unlock(&global_arena_lock);
}
