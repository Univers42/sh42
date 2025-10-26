include ../build/common.mk
include ../build/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I./ft_printf/include
DEPFLAGS = -MMD -MP

NAME_LIB	= 		stdlib
NAME		= 		$(NAME_LIB).a
SRCS		=		ft_atoi_base.c ft_atoi.c ft_calloc.c ft_dbl_free.c ft_itoa.c \
					ft_realloc.c ft_utoa.c ft_xtoa.c randoms.c seeds.c skip_spaces.c \
					utils_rand.c
SRCS		+=  	$(addprefix slab/, slab_alloc.c slab_destroy.c slab_free.c \
					slab_helpers.c slab_stats.c slab.c)
SRCS		+=  	$(addprefix pool/, pool_helpers.c pool_stats.c pool.c)
SRCS 		+=      $(addprefix arena/, arena.c)
SRCS		+=		$(addprefix strto/, conv.c helpers.c state_conv.c \
					stub1.c stub2.c stub3.c stub4.c)

OBJ_DIR		=		obj
TEST_DIR	=		unit_test

TEST_SRCS	=		$(TEST_DIR)/test.c
TEST_OBJS	=		$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(NAME)...)
	@$(AR) $(ARFLAGS) $@ $^ >/dev/null 2>&1
	$(call print_status,$(GREEN),DONE,Library $(NAME) successfully created!)

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(call create_dirs)
	@mkdir -p $(dir $@)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(call create_dirs)

$(OBJ_DIR)/$(TEST_DIR):
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build test object
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR)/$(TEST_DIR)
	@$(call create_dirs)
	@mkdir -p $(dir $@)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

# Test target
test: $(NAME) $(TEST_OBJS)
	$(call print_status,$(BLUE),TEST,Building test runner...)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_BIN)
	$(call print_status,$(GREEN),TEST,Test binary ready: ./$(TEST_BIN))
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(call clean_objects)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binary...)
	@$(RM) $(NAME) $(TEST_BIN)

re: fclean all

-include $(DPDC)