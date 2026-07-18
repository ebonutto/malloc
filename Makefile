ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK_NAME := libft_malloc.so
EXEC := malloc

SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BUILD_DIR := .build

CC := cc
CFLAGS := -Wall -Wextra -Werror -fPIC
CPPFLAGS := -I$(INC_DIR) -MMD -MP
LDFLAGS := -shared
LDLIBS := -lpthread

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean fclean re run

all: $(LINK_NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(LINK_NAME): $(NAME)
	ln -sf $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(EXEC): $(LINK_NAME) $(TEST_DIR)/main.c
	$(CC) -I$(INC_DIR) $(CFLAGS) $(TEST_DIR)/main.c -o $@

run: all $(EXEC)
	LD_PRELOAD=./$(LINK_NAME) ./$(EXEC)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME) $(LINK_NAME) $(EXEC)

re: fclean all

-include $(DEPS)
