CC = gcc

SRCS = main.c src/allocator.c include/types/arena.h include/types/block.h
TARGET = main.o

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) -d

clean:
	rm -f $(TARGET)

.PHONY: run clean