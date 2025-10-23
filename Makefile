# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/23 19:03:21 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/24 01:47:06 by syzygy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Generic project Makefile — discovers sources in srcs, builds objects in .objs/, and includes dependency files

# Submodule / dependency management
SUBMODULE_DIR := libft
SUBMODULE_REPO := git@github.com:Univers42/libft.git
SUBMODULE_LIB := $(SUBMODULE_DIR)/libft.a

.PHONY: all libft-a clean fclean re

include variables.mk
-include libft/build/colors.mk
-include libft/build/common.mk

SRCDIR := srcs
OBJDIR := .objs
DEPDIR := .deps

SRCS := $(shell find $(SRCDIR) -type f -name '*.c' -print)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$(OBJS))

# detect libft archive at link-time (evaluated when used)
LIBFT_A = $(firstword $(wildcard $(SUBMODULE_DIR)/libft.a $(SUBMODULE_DIR)/build/libft.a $(SUBMODULE_DIR)/lib/libft.a))

MINISHELL_A := libminishell.a

# Default target: build the program (libft will be ensured first)
all: $(NAME)

# Ensure libft.a exists; if not, try to build it using the libft Makefile (simple and explicit)
LIBFT_STAMP := .libft.stamp

$(LIBFT_STAMP): $(SUBMODULE_LIB)
	@echo "[deps] libft.a is now available"
	@touch $@

$(SUBMODULE_LIB):
	@echo "[deps] ensuring libft.a..."
	@if [ -f "$(SUBMODULE_LIB)" ]; then \
		echo "[deps] libft.a found: $(SUBMODULE_LIB)"; \
	else \
		if [ -d "$(SUBMODULE_DIR)" ]; then \
			if [ -f "$(SUBMODULE_DIR)/Makefile" ]; then \
				echo "[deps] building libft in $(SUBMODULE_DIR)"; \
				$(MAKE) -C $(SUBMODULE_DIR) || true; \
			else \
				echo "[deps] libft directory exists but no Makefile — cannot build automatically"; \
			fi; \
		else \
			echo "[deps] libft not present — trying to init submodule"; \
			git submodule update --init --recursive $(SUBMODULE_DIR) || git submodule add $(SUBMODULE_REPO) $(SUBMODULE_DIR) || true; \
			if [ -f "$(SUBMODULE_DIR)/Makefile" ]; then \
				echo "[deps] building libft in $(SUBMODULE_DIR)"; \
				$(MAKE) -C $(SUBMODULE_DIR) || true; \
			fi; \
		fi; \
	fi; \
	if [ ! -f "$(SUBMODULE_LIB)" ]; then \
		echo "[deps] libft.a still missing — continuing without libft (link may fail)"; \
	fi; \
	if [ -z "$$MAKE_RESTARTED" ] && { [ -f "$(SUBMODULE_DIR)/build/common.mk" ] || [ -f "$(SUBMODULE_DIR)/build/colors.mk" ]; }; then \
		echo "[deps] libft: build helpers available; re-running make to load them (once)"; \
		$(MAKE) MAKE_RESTARTED=1 all; \
		exit 0; \
	fi

# Build static library from all project objects
$(MINISHELL_A): $(OBJS)
	@echo "[AR] $@"
	@ar rcs $@ $(OBJS)

# Link the final binary from your static lib and libft
$(NAME): $(LIBFT_STAMP) $(MINISHELL_A)
	$(call print_status,$(GREEN),LINK,$@)
	@echo "[link] linking $(NAME)"
	$(CC) $(CFLAGS) $(OPTFLAGS) -o $@ $(MINISHELL_A) $(LIBFT_A) $(LDFLAGS)

# Compile C source to object files and emit dependency files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@) $(DEPDIR)/$(dir $*)
	$(call print_status,$(CYAN),COMPILING,$<)
	@$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@ -MMD -MP -MF $(DEPDIR)/$*.d -I$(SUBMODULE_DIR)/include -I./incs

# Include dependency files if present
-include $(DEPS)

clean:
	$(call print_status,$(RED),CLEAN,Removing objects and archives)
	@rm -rf $(OBJDIR)
	@rm -rf $(DEPDIR)
	@rm -f .libft.stamp
	@rm -f $(MINISHELL_A)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing binary)
	@rm -f $(NAME)
	@if [ -d "$(SUBMODULE_DIR)" ]; then $(MAKE) -C $(SUBMODULE_DIR) fclean || true; fi

re: fclean all

.PHONY: all clean fclean re