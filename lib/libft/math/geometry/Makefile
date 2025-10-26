# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../../build/common.mk
include ../../build/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../../include -I../algebra -I../algebra/include -I./draw -I./matrices
DEPFLAGS	= 		-MMD -MP

NAME_LIB	= 		string
NAME		= 		$(NAME_LIB).a		
SRCS		=  		$(addprefix ./draw/, collision.c colors.c ft_bending.c  \
						gradient.c lerp_point.c translate.c valid_pixel.c z_div.c)
SRCS		+=		$(addprefix ./matrices/, matrix_from_quaternion.c matrix_mult.c matrix_copy.c \
						matrix_identity.c matrix_init.c matrix_rotate.c matrix_scale.c matrix_translate.c \
						matrix_transpose.c matrix_transpose.c matrix_vec_multiply.c matrix_zero.c)
SRCS		+=		$(addprefix ./quaternion/, add_quats.c axis_to_quat.c normalize_quat.c point_trackball.c \
						quat_from_axis_angle.c quat_init.c quat_mult.c quat_normalize.c quat_slerp.c quat_to_matrix.c\
						quat_to_matrix4.c rot_quatmatrix.c trackball_rotate.c trackball.c vadd.c vcopy.c vcross.c \
						vdot.c vlength.c vnormal.c vscale.c vset.c vsub.c vzero.c)

TOTAL_OBJS := $(words $(SRCS))

OBJ_DIR		=		obj
TEST_DIR	=		unit_test

TEST_SRCS	=		$(TEST_DIR)/test.c
TEST_OBJS	=		$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

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

# Path to the test framework static library (adjust if needed)
TEST_FRAMEWORK_DIR = ../testing
TEST_FRAMEWORK_LIB = $(TEST_FRAMEWORK_DIR)/libtest_framework.a
LIBFT = ../libft.a

# Test target
test: $(NAME) $(TEST_OBJS) $(TEST_FRAMEWORK_LIB) $(LIBFT)
	$(call print_status,$(BLUE),TEST,Building test runner...)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) $(TEST_FRAMEWORK_LIB) $(LIBFT) -o $(TEST_BIN)
	$(call print_status,$(GREEN),TEST,Test binary ready: ./$(TEST_BIN))
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(call clean_objects)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binary...)
	@$(RM) $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test

-include $(DPDC)