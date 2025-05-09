#include "../include/arena_allocator.h"

Arena** arenas = NULL;
static __thread Arena* thread_arena = NULL;
static Arena* main_arena = NULL;
static Block* TCache[TCACHE_MAX_BINS];
static size_t page_size = 4096;

int num_arenas = 0;
int max_arenas = 0;

pthread_mutex_t arenas_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t global_arena_lock = PTHREAD_MUTEX_INITIALIZER;

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
    arena->top_chunk = chunk; 
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
    pthread_mutex_lock(&arena->lock);
    init_bins(arena);
    arenas[num_arenas] = arena;
    num_arenas++;

    pthread_mutex_unlock(&arena->lock);
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
    if (arenas == NULL) {
        init_arenas();
    }
    printf("\n\n");
}

Arena* get_arena() {
    if (thread_arena != NULL) {
        return thread_arena;
    }

    pthread_mutex_lock(&global_arena_lock);

    for (int i = 0; arenas[i] != NULL; i++) {
        if (pthread_mutex_trylock(&arenas[i]->lock) == 0) {
            pthread_mutex_unlock(&global_arena_lock);
            printf("Found previously created Arena\n");
            return arenas[i];
        }
    }

    if (num_arenas < max_arenas) {
        Arena* new_arena = arenas[num_arenas];
        new_arena = create_new_arena();
        pthread_mutex_unlock(&global_arena_lock);
        pthread_mutex_lock(&new_arena->lock);
        thread_arena = new_arena;
        arenas[num_arenas] = new_arena;

        num_arenas++;

        printf("Created new Arena\n");
        
        return new_arena;
    }

    pthread_mutex_lock(&main_arena->lock);
    pthread_mutex_unlock(&global_arena_lock);
    printf("Returning Main Arena\n");
    return main_arena;
}

void* new(size_t size) {
    pthread_t thread_id = pthread_self();

    Arena* arena = get_arena();
    printf("Arena pointer: %p\n", arena);
    void* ptr = allocate_block(arena, size);

    pthread_mutex_unlock(&arena->lock);

    return ptr;
}

void free(void* ptr) {
    free_block(ptr);
}