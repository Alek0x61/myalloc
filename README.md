https://sourceware.org/glibc/wiki/MallocInternals

https://gist.github.com/attractivechaos/862fb6e58147b47c9d16bf2d9e12445f

https://azeria-labs.com/heap-exploitation-part-1-understanding-the-glibc-heap-implementation/


Inspired by https://www.youtube.com/watch?v=CulF4YQt6zA

TODO:
1. Implement free(void* ptr).
2. Implement realloc(void* ptr, size_t size).
3. Implement calloc(size_t nmemb, size_t size).
4. Implement block splitting when a large free block is used for a smaller allocation.
5. Implement block coalescing.
6. Implement fast bins, unsorted bins, small bins, and large chunks. (Optional)