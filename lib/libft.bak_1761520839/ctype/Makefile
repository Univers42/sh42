# friendly, readable Makefile for the ctype module

# reuse global common helpers (colors, print_status, create_dirs, etc.)
BUILD_DIR := ../build
include $(BUILD_DIR)/common.mk

# toolchain
CC ?= $(CC)
CFLAGS += -I../include
DEPFLAGS = -MMD -MP

NAME = ctype.a

SRCS = \
	ft_hex_digit.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isblank.c \
	ft_isdigit.c \
	ft_isgraph.c \
	ft_islower.c \
	ft_isprint.c \
	ft_isspace.c \
	ft_isupper.c \
	ft_isxdigit.c \
	ft_tolower.c \
	ft_toupper.c	\
	is_at_end.c		\
	assert_char.c	\
	is_word_char.c	\
	is_meta_char.c	\
	is_name_char.c

OBJ_DIR := obj
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

TESTOBJ := $(OBJ_DIR)/test.o
TESTBIN := test_ctype

TEST_FRAMEWORK_LIB := ../testing/libtest_framework.a
LIBFT := ../libft.a

.PHONY: all test clean fclean re

# default
all: pre_build $(NAME)

pre_build:
	$(call create_dirs)
	$(call print_status,$(CYAN),SETUP,Preparing ctype build...)

# archive library
$(NAME):
	$(call print_status,$(MAGENTA),ARCHIVE,Ensuring object files and creating $@)
	@set -e; \
	for src in $(SRCS); do \
		srcpath=$$src; \
		obj="$(OBJ_DIR)/$${srcpath%.c}.o"; \
		if [ ! -f "$$obj" ]; then \
			printf "[COMP] building %s -> %s\n" "$$srcpath" "$$obj"; \
			$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(DEPFLAGS) -c "$$srcpath" -o "$$obj"; \
		fi; \
	done; \
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(call print_status,$(GREEN),DONE,$@ created)

# compile rule
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(call print_status,$(YELLOW),COMP,Compiling $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(DEPFLAGS) -c $< -o $@

# Test binary (links ctype + test framework + libft)
$(TESTBIN): $(OBJS) $(TESTOBJ)
	$(call print_status,$(MAGENTA),LINK,Linking test binary)
	@$(CC) $(CFLAGS) $^ $(TEST_FRAMEWORK_LIB) $(LIBFT) -o $@
	$(call print_status,$(GREEN),DONE,Test binary ready: $@)

test: $(TESTBIN)
	$(call print_status,$(CYAN),TEST,Running ctype tests)
	@./$(TESTBIN)

# helpers to ensure top-level libft.a exists when requested
$(LIBFT):
	$(call print_status,$(YELLOW),LIB,Requesting top-level libft.a)
	@$(MAKE) -C .. libft.a

# Clean
clean:
	$(call print_status,$(RED),CLEAN,Removing object files)
	@rm -rf $(OBJ_DIR)

fclean: clean
	$(call print_status,$(RED),FCLEAN,Removing libraries and test binary)
	@rm -f $(NAME) $(TESTBIN)

re: fclean all

# include dependency files
-include $(DEPS)