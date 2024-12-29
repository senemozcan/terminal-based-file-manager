# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

# Target executable and symbolic links
TARGET = $(BIN_DIR)/scli
LINKS = listdir createdir removedir createfile removefile copyfile movefile printfile searchfile setpermission

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJ_DIRS = $(sort $(dir $(OBJS)))

# Default target
all: $(BIN_DIR) $(OBJ_DIRS) $(TARGET) links

# Create necessary directories
$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIRS):
	mkdir -p $@

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	echo "Compiled: $@"

# Link object files to create the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	echo "Linked: $(TARGET)"

# Create symbolic links in the bin directory
links: $(TARGET)
	@for link in $(LINKS); do \
		ln -sf scli $(BIN_DIR)/$$link; \
		echo "Created symbolic link: $(BIN_DIR)/$$link -> scli"; \
	done

# Clean up build files
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	echo "Cleaned up: $(BIN_DIR) and $(OBJ_DIR)"

# Debug target to print variables (optional)
debug:
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Object directories: $(OBJ_DIRS)"

.PHONY: all links clean debug

