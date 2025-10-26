# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

CC			= 		cc
CFLAGS		= 		-Wall -Wextra -Werror -pedantic -Wshadow -std=c99
INCLUDE_FLAGS := -I$(abspath ../include)
DEPFLAGS	= 		-MMD -MP
NAME_LIB	= 		debug
NAME		= 	$(NAME_LIB).a
SRCS		=	ft_print_array.c ft_print_ascii.c ft_print_hex.c ft_print_list.c ft_print_memory.c
OBJ_DIR		=	obj
TEST_DIR	=	unit_test

TEST_SRCS	=	$(TEST_DIR)/test.c
TEST_OBJS	=	$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=	test_$(NAME_LIB)

OBJS		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))
RM			:=	rm -rf

# reuse global common helpers
BUILD_DIR := ../build
include $(BUILD_DIR)/common.mk

all: pre_build $(NAME)

pre_build:
	$(call create_dirs)
	$(call print_status,$(CYAN),SETUP,Preparing debug build...)

$(NAME): $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $@)
	@$(AR) $(ARFLAGS) $@ $^
	$(call print_status,$(GREEN),DONE,$@ created)

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(call print_status,$(YELLOW),COMP,Compiling $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/$(TEST_DIR):
	mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build test object
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR)/$(TEST_DIR)
	@mkdir -p $(dir $@)
	$(call print_status,$(YELLOW),COMP,Compiling test $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -I. -c $< -o $@

# Test target
test: $(NAME) $(TEST_OBJS)
	$(call print_status,$(MAGENTA),LINK,Linking test binary)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_BIN)
	$(call print_status,$(GREEN),TEST,Run ./$(TEST_BIN) to execute tests.)

clean:
	$(call print_status,$(RED),CLEAN,Removing object files)
	@$(RM) $(OBJ_DIR)

fclean: clean
	$(call print_status,$(RED),FCLEAN,Removing libraries and test binary)
	@$(RM) $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test

-include $(DPDC)