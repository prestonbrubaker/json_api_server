# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I include -Wno-unused-parameter
LIBS = -lmicrohttpd

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/json_server

# Default rule
all: $(TARGET)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

# Run the server
run: all
	./$(TARGET)

# Install dependencies (Ubuntu)
install:
	sudo apt update && sudo apt install -y libmicrohttpd-dev
