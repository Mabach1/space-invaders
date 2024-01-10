CC := gcc
CFLAGS := -g -O3 -Wall -Wextra -Wpedantic -Werror -fsanitize=address -lSDL2 -lSDL2_image
SRC_DIR := src
BUILD_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.DEFAULT_GOAL := all

.PHONY: directories

directories: ${BUILD_DIR}

${BUILD_DIR}:
	@mkdir ${BUILD_DIR}

$(BUILD_DIR)/main: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm $(OBJS) -f
	@rm $(BUILD_DIR)/main -f

.PHONY: all
all: directories $(BUILD_DIR)/main

.PHONY: force-rebuild
force-rebuild: clean all