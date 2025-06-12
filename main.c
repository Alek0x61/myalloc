#include <stdio.h>
#include "include/arena_allocator.h"

int main() {
    int* ptr = new(500);
    //free_block(ptr);

    printf("ptr: %p\n", ptr);
}


// #include <stdio.h>
// #include "include/arena_allocator.h"

// void* thread_func(void* arg) {
//     int* ptr = new(450);
//     test();
//     test();
//     return NULL;
// }

// int main() {
//     int thread_number = 2;

//     pthread_t threads[thread_number];
//     for (long i = 0; i < thread_number; ++i) {
//         pthread_create(&threads[i], NULL, thread_func, (void*)i);
//     }

//     for (int i = 0; i < thread_number; ++i) {
//         pthread_join(threads[i], NULL);
//     }


//     //free_block(ptr);

//     //printf("ptr: %p\n", ptr);
// }