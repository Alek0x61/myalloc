#include <stdio.h>
#include "include/arena_allocator.h"

int main() {
    int* ptr = new(450);
    free_block(ptr);

    printf("ptr: %p\n", ptr);
}