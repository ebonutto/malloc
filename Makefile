################################################################################
# Project
################################################################################

MAKEFLAGS += --no-print-directory

HOSTTYPE := $(shell uname -m)_$(shell uname -s | tr '[:upper:]' '[:lower:]')

NAME      := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so
TEST      := malloc

################################################################################
# Directories
################################################################################

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := .build
TEST_DIR  := tests

################################################################################
# Compiler
################################################################################

CC := cc
RM := rm -rf

CFLAGS   := -Wall -Wextra -Werror
CPPFLAGS := -I$(INC_DIR) -MMD -MP
LDFLAGS  :=
LDLIBS   := -lpthread

CFLAGS += -fPIC

ifdef DEBUG
	CFLAGS += -g3
endif

ifdef SAN
	CFLAGS  += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

################################################################################
# Sources
################################################################################

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

################################################################################
# Rules
################################################################################

.PHONY: all clean fclean re test run

all: $(LINK_NAME)

$(NAME): $(OBJS)
	$(CC) -shared $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(LINK_NAME): $(NAME)
	ln -sf $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST): $(NAME)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TEST_DIR)/main.c -o $@ $(LDFLAGS)

run: $(TEST)
	LD_PRELOAD=./$(LINK_NAME) ./$(TEST)

test: run

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LINK_NAME)
	$(RM) $(TEST)

re: fclean all

-include $(DEPS)