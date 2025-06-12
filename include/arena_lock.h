#ifndef ARENA_LOCK_H
#define ARENA_LOCK_H

#include "types/arena.h"

#include <pthread.h>

void init_arena_lock(Arena* arena);
void lock_arena(Arena* arena);
void unlock_arena(Arena* arena);

void init_global_lock();
void global_lock();
void global_unlock();

#endif