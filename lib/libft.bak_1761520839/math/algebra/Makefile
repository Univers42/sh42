# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../../build/common.mk
include ../../build/debug.mk

# project-specific settings
HEADERS_DIR = $(shell find . -type d)
CFLAGS += -std=c99 -pedantic -Wshadow -I../../include
DEPFLAGS = -MMD -MP

NAME_LIB	= 		algebra
NAME		= 	$(NAME_LIB).a

SRCS		=	ease-in-out.c ft_abs.c ft_add_prime_sum.c ft_add.c ft_divide.c \
				ft_fabs.c ft_fmaxf.c ft_fminf.c ft_fprime.c ft_hcf.c ft_is_negative.c ft_is_power_of_2.c \
				ft_isprime.c ft_lcm.c ft_lerp.c ft_log10.c ft_max.c ft_min.c ft_modulo.c ft_multiply.c \
				ft_round.c ft_sqrt.c ft_substract.c ft_tab_mult.c get_raw_color.c lerp_chan.c
TOTAL_OBJS := $(words $(SRCS))

OBJ_DIR		=	obj
TEST_DIR	=	unit_test

TEST_SRCS	=	$(TEST_DIR)/test.c
TEST_OBJS	=	$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=	test_$(NAME_LIB)

OBJS		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

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
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_BIN)
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(call clean_objects)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binary...)
	@$(RM) $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test

-include $(DPDC)