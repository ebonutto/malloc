# Project
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s | tr '[:upper:]' '[:lower:]')
NAME := libft_malloc_$(HOSTTYPE).so
NAME_LINK := libft_malloc.so
TEST_BIN := malloc

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := .build
TEST_DIR := tests

# Compiler
CC := cc
CFLAGS := -Wall -Werror -Wextra -fPIC
CPPFLAGS := -I$(INC_DIR) -MMD -MP
LDFLAGS := 
LDLIBS := -lpthread

# Sources
SRCS := $(shell find $(SRC_DIR) -type f -name "*.c" ! -name "main.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Build rules
.PHONY: all test clean fclean re

all: $(NAME_LINK)

$(NAME): $(OBJS)
	$(CC) -shared $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(NAME_LINK): $(NAME)
	@ln -sf $(NAME) $(NAME_LINK)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TEST_BIN): $(NAME_LINK)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TEST_DIR)/main.c -L. -lft_malloc -o $@

test: $(TEST_BIN)
	LD_PRELOAD=./$(NAME_LINK) ./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_LINK) $(TEST_BIN)

re: fclean all

-include $(DEPS)
