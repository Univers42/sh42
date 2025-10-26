NAME = test_framework
CC = cc
CFLAGS = -Wall -Wextra -Werror -I../include

OBJ_DIR = obj
AR ?= ar
ARFLAGS ?= rcs
RM = rm -rf
LIBFT = ../libft.a

SRC_COMMON = \
	helpers.c \
	runners.c \
	report.c \
	validators.c \
	test.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_COMMON:.c=.o))
MAIN_OBJ = $(OBJ_DIR)/main.o
MULTI_OBJ = $(OBJ_DIR)/test_multi.o

STATIC_LIB = libtest_framework.a

all: $(NAME) $(STATIC_LIB)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MULTI_OBJ): test_multi.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(MAIN_OBJ) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) $(OBJS) $(LIBFT)

test_multi: $(MULTI_OBJ) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(MULTI_OBJ) $(OBJS) $(LIBFT)

$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(LIBFT):
	# Ensure consolidated ../libft.a exists by invoking the libft.a target at repo root
	$(MAKE) -C .. libft.a

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME) test_multi $(STATIC_LIB)

re: fclean all

.PHONY: all clean fclean re test_multi
