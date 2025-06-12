# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2 -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN := myalloc

# Find all .c files in src directory and subdirectories
SRC_FILES := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
MAIN_OBJ := $(OBJ_DIR)/main.o

# Default target
all: $(BIN)

# Link final binary
$(BIN): $(OBJ_FILES) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile main separately
$(MAIN_OBJ): main.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# Phony targets
.PHONY: all clean
