#include "../include/arena_allocator.h"

Arena** arenas = NULL;
static __thread Arena* thread_arena = NULL;
static Arena* main_arena = NULL;
//static size_t page_size = 4096;

int num_arenas = 0;
int max_arenas = 0;

void init_bins(Arena* arena) {
    Block* chunk = request_top_chunk_memory();
    size_t size = BLOCK_SIZE;
    size = ALIGN_SIZE(size);
    for (size_t i = 0; i < NUM_FASTBINS; i++)
    {
        arena->fastbins[i] = split_chunk(&chunk, size);
        arena->fastbins[i]->size = size;
        size += ALIGNER_BIT; 
    }
    for (size_t i = 0; i < SMALLBIN_THRESHOLD; i++)
    {
        arena->bins[i] = split_chunk(&chunk, size);
        arena->bins[i]->size = size;
        size += ALIGNER_BIT; 
    }
    printf("top_chunk size: %d\n", size);
    arena->top_chunk = chunk; 
}

Arena* create_new_arena() {
    Arena* arena = mmap(NULL, ALIGN_SIZE_64BIT(sizeof(Arena)), 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);

    if (arena == MAP_FAILED) {
        perror("mmap failed to allocate new arena");
        return NULL; 
    }
    
    return arena;
}

int get_max_arenas() {
    static int cached = 0;
    if (cached == 0) {
        long cores = sysconf(_SC_NPROCESSORS_ONLN);
        cached = (cores > 0) ? cores * 8 : 8;
    }
    return cached;
}

void init_arenas() {
    max_arenas = get_max_arenas();
    arenas = mmap(NULL, sizeof(Arena*) * max_arenas, 
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);
    printf("Arenas initialized at address: %p\n", arenas);

    if (arenas == MAP_FAILED) {
        perror("Failed to allocate arenas");
    }

    for (int i = 0; i < max_arenas; i++) {
        arenas[i] = NULL;
    }
    printf("Trying to create main arena\n");
    main_arena = create_new_arena();
    printf("Main arena created\n");
    if (!main_arena) {
        fprintf(stderr, "Failed to allocate main_arena\n");
        exit(EXIT_FAILURE);
    }
}

__attribute__((constructor))
static void allocator_constructor() {
    init_global_lock();
    if (arenas == NULL) {
        init_arenas();
    }
    printf("\n\n");
}

Arena* get_arena() {
    if (thread_arena != NULL) {
        return thread_arena;
    }

    global_lock();

    for (int i = 0; arenas[i] != NULL; i++) {
        if (pthread_mutex_trylock(&arenas[i]->lock) == 0) {
            global_unlock();
            printf("Found previously created Arena\n");
            return arenas[i];
        }
    }

    if (num_arenas < max_arenas) {
        Arena* new_arena = arenas[num_arenas];
        new_arena = create_new_arena();
        init_arena_lock(new_arena);

        global_unlock();
        
        init_bins(new_arena);
        arenas[num_arenas] = new_arena;
        thread_arena = new_arena;

        num_arenas++;

        printf("Created new Arena\n");
        
        return thread_arena;
    }

    global_unlock();
    printf("Returning Main Arena\n");
    return main_arena;
}

void* allocate_block(Arena* arena, size_t size) {
    size += BLOCK_SIZE;
    size = ALIGN_SIZE(size);

    printf("Trying to allocate size: %d\n", size);

    int index = tcache_find_index(size);
    if (index > -1) {
        return tcache_alloc(index);
    }

    if (size >= M_MMAP_THRESHOLD) {
        size_t metadata_size = ENCODE_AS_MMAPPED(size);
        Block* block = request_block_memory(metadata_size, size);
        return block;
    }

    lock_arena(arena);

    index = fastbin_find_index(arena, size);
    if (index > -1) {
        Block* block = fastbin_alloc(arena, index);
        unlock_arena(arena);
        return block;
    }

    index = smallbin_find_index(arena, size);
    if (index > -1) {
        Block* block = smallbin_alloc(arena, index);
        unlock_arena(arena);
        return block;
    }

    Block* unsortedbin_block = unsortedbin_try_alloc(arena, size);
    if (unsortedbin_block != NULL) {
        unlock_arena(arena);
        return unsortedbin_block;
    }

    index = largebin_find_index(arena, size);
    if (index > -1) {
        Block* block = largebin_alloc(arena, index, size);
        unlock_arena(arena);
        return block;
    }

    //TODO sort unsorted blocks into small and large bins
    if (arena->top_chunk->size < size) {
        Block* top_chunk_prev = arena->top_chunk;
        arena->top_chunk = request_top_chunk_memory();
        arena->top_chunk = combine_two_chunks(top_chunk_prev, arena->top_chunk);
    }
    printf("Splitting from top chunk\n");
    Block* block = split_chunk(&arena->top_chunk, size);

    unlock_arena(arena);

    return block;
}


void* new(size_t size) {
    pthread_t thread_id = pthread_self();

    Arena* arena = get_arena();
    printf("Arena pointer: %p\n", arena);
    void* block_header = allocate_block(arena, size);
    void* usable_memory = (void*)(block_header + 1);

    return usable_memory;
}

void free(void* ptr) {
    // free_block(ptr);
}