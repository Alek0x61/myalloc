#include "../include/allocator.h"

Arena** arenas = NULL;
static __thread Arena* thread_arena = NULL;
static Arena* main_arena = NULL;

int num_arenas = 0;
int max_arenas = 0;

pthread_mutex_t arenas_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t global_arena_lock = PTHREAD_MUTEX_INITIALIZER;

int get_max_arenas() {
    static int cached = 0;
    if (cached == 0) {
        long cores = sysconf(_SC_NPROCESSORS_ONLN);
        cached = (cores > 0) ? cores * 8 : 8;
    }
    return cached;
}

void init_arenas() {
    if (arenas == NULL) {
        max_arenas = get_max_arenas();
        arenas = mmap(NULL, sizeof(Arena*) * max_arenas, 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);

        if (arenas == MAP_FAILED) {
            perror("Failed to allocate arenas");
        }

        for (int i = 0; i < max_arenas; i++) {
            arenas[i] = NULL;
        }

        main_arena = create_new_arena();
        if (!main_arena) {
            fprintf(stderr, "Failed to allocate main_arena\n");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&main_arena->lock);
        arenas[0] = main_arena;
        num_arenas = 1;

        pthread_mutex_unlock(&main_arena->lock);
    }
}

__attribute__((constructor))
static void allocator_constructor() {
    init_arenas();
    printf("Arenas init\n");
}

Arena* create_new_arena() {
    Arena* arena = mmap(NULL, sizeof(Arena), 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);

    if (arena == MAP_FAILED) {
        perror("mmap failed to allocate new arena");
        return NULL; 
    }
}

Arena* get_arena() {
    // return already allocated thread arena, if it exists
    if (thread_arena != NULL) {
        return thread_arena;
    }

    pthread_mutex_lock(&global_arena_lock);

    // search for unlocked already allocated arena
    for (int i = 0; arenas[i] != NULL; i++) {
        if (pthread_mutex_trylock(&arenas[i]->lock) == 0) {
            pthread_mutex_unlock(&global_arena_lock);
            return arenas[i];
        }
    }

    // allocate new arena
    if (num_arenas < max_arenas) {
        Arena* new_arena = arenas[num_arenas];
        new_arena = create_new_arena();
        pthread_mutex_unlock(&global_arena_lock);
        pthread_mutex_lock(&new_arena->lock);
        thread_arena = new_arena;
        arenas[num_arenas] = new_arena;

        num_arenas++;
        
        return new_arena;
    }

    // If everything fails, we gotta use the main arena
    pthread_mutex_lock(&main_arena->lock);
    pthread_mutex_unlock(&global_arena_lock);
    return main_arena;

    // if (pthread_mutex_trylock(&main_arena->lock) == 0) {
    //     pthread_mutex_unlock(&global_arena_lock);
    //     thread_arena = main_arena;
    //     return main_arena;
    // }
}

Block* find_free_block(Arena* arena, size_t size) {
    Block* curr = arena->blockList;
    Block* result = NULL;

    int i = 0;
    while (curr) {
        printf("Arena %p, checking if block with index %d is free\n", arena, i);

        if (curr->free && curr->size >= size) {
            result = curr;
        }
        curr = curr->fd;

        i++;
    }
    
    return result;
}

Block* request_block_memory(size_t size) {
    size_t actual_size = size + sizeof(Block);
    Block* block = mmap(NULL, actual_size, 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);
    
    if (block == MAP_FAILED)
        return NULL;

    block->size = size;
    block->free = 0;
    block->fd = NULL;

    return block;
}

void* allocate_block(Arena* arena, size_t size) {
    Block *block = find_free_block(arena, size);
    if (!block) {
        printf("REQUESTING MEMORY\n");
        block = request_block_memory(size);
        if (!block)
            return NULL;

        if (!arena->blockList) {
            arena->blockList = block;
        } else {
            Block* temp = arena->blockList;
            while (temp->fd) {
                temp = temp->fd;
            }
            temp->fd = block;
            block->bk = temp;
        }
    } else {
        printf("FOUND THE EXISTING BLOCK\n");
        block->free = 0;
    }

    return (void*)(block + 1);
}

void* new(size_t size) {
    pthread_t thread_id = pthread_self();

    Arena* arena = get_arena();
    allocate_block(arena, size);

    pthread_mutex_unlock(&arena->lock);
    printf("Thread ID: %lu, arena pointer: %p\n\n", (unsigned long)thread_id, arena);
}