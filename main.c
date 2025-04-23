#include <stdio.h>
#include "include/allocator.h"

void* thread_func(void* arg) {
    printf("Starting new thread\n");
    void* another_thread_pointer1 = new(16);
    void* another_thread_pointer2 = new(16);
    return NULL;
}

int main() {
    void* main_thread_pointer1 = new(16);
    void* main_thread_pointer2 = new(16);

    int thread_number = 1;

    pthread_t threads[thread_number];
    for (long i = 0; i < thread_number; ++i) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    for (int i = 0; i < thread_number; ++i) {
        pthread_join(threads[i], NULL);
    }
}