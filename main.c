#include <stdio.h>
#include "include/arena_allocator.h"

int main() {
    int* ptr = new(sizeof(int));

    printf("ptr: %p\n", ptr);
}