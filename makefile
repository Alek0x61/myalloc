CC = gcc

SRCS = main.c src/arena_allocator.c src/block_allocator.c src/helpers/mmap_helper.c include/types/arena.h include/types/block.h include/helpers/memory_aligner.h
TARGET = main.o

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) -d

clean:
	rm -f $(TARGET)

.PHONY: run clean