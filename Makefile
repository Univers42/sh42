# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/23 19:03:21 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/24 00:46:30 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Generic project Makefile — discovers sources in srcs, builds objects in .objs/, and includes dependency files


# Submodule / dependency management
SUBMODULE_DIR := libft
SUBMODULE_REPO := git@github.com:Univers42/libft.git
SUBMODULE_LIB := $(SUBMODULE_DIR)/libft.a

.PHONY: deps check-submodule sync-libft
# Default target
all: deps $(NAME)

include variables.mk
-include libft/build/colors.mk
-include libft/build/common.mk

# Ensure submodule is present, updated to latest remote and built if it provides a Makefile
deps: check-submodule

check-submodule:
	@mkdir -p .deps
	@STAMP=.deps/libft.last; \
	if [ -f "$$STAMP" ]; then \
		LAST=$$(stat -c %Y "$$STAMP" 2>/dev/null || echo 0); \
	else \
		LAST=0; \
	fi; \
	NOW=$$(date +%s); \
	DIFF=$$((NOW - LAST)); \
	if [ "$$DIFF" -lt 86400 ]; then \
		echo "[deps] libft: recently synced (within 24h)"; \
		if [ ! -d "$(SUBMODULE_DIR)" ]; then \
			git submodule update --init --remote --recursive $(SUBMODULE_DIR) || true; \
		fi; \
		if [ ! -f "$(SUBMODULE_DIR)/libft.a" ]; then \
			if [ -f "$(SUBMODULE_DIR)/Makefile" ]; then \
				$(MAKE) -C $(SUBMODULE_DIR) -s || true; \
			fi; \
			if [ -f "$(SUBMODULE_DIR)/libft.a" ]; then \
				echo "[deps] libft: built"; \
			else \
				echo "[deps] libft: build failed"; \
			fi; \
		fi; \
	else \
		echo "[deps] libft: syncing..."; \
		if [ ! -d "$(SUBMODULE_DIR)" ]; then \
			echo "Initializing submodule '$(SUBMODULE_DIR)'..."; \
			git submodule update --init --remote --recursive $(SUBMODULE_DIR) || git submodule add $(SUBMODULE_REPO) $(SUBMODULE_DIR); \
		fi; \
		git -C $(SUBMODULE_DIR) fetch --all --tags --prune --quiet || true; \
		git -C $(SUBMODULE_DIR) pull --ff-only --quiet || true; \
		NEW=$$(git -C $(SUBMODULE_DIR) rev-parse --short HEAD 2>/dev/null || echo none); \
		echo "[deps] libft: synced $$NEW"; \
		if [ ! -f "$(SUBMODULE_DIR)/libft.a" ]; then \
			if [ -f "$(SUBMODULE_DIR)/Makefile" ]; then \
				$(MAKE) -C $(SUBMODULE_DIR) -s || true; \
			fi; \
			if [ -f "$(SUBMODULE_DIR)/libft.a" ]; then \
				echo "[deps] libft: built"; \
			else \
				echo "[deps] libft: build failed"; \
			fi; \
		fi; \
		touch "$$STAMP"; \
	fi; \
	if [ -f "libft/build/common.mk" ] || [ -f "libft/build/colors.mk" ]; then \
		if [ -z "$$MAKE_RESTARTED" ]; then \
			echo "[deps] libft: reloading Make to include libft build helpers"; \
			$(MAKE) MAKE_RESTARTED=1 $(MAKECMDGOALS); \
			exit 0; \
		fi; \
	fi

sync-libft:
	@git submodule update --init --remote --recursive $(SUBMODULE_DIR)
	@git -C $(SUBMODULE_DIR) pull --ff-only || true

SRCDIR := srcs
OBJDIR := .objs

SRCS := $(shell find $(SRCDIR) -type f -name '*.c' -print)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# path to libft archive provided by the submodule (detect common locations)
SUBMODULE_LIB := $(SUBMODULE_DIR)/libft.a
LIBFT_A := $(firstword $(wildcard $(SUBMODULE_DIR)/libft.a $(SUBMODULE_DIR)/build/libft.a $(SUBMODULE_DIR)/lib/libft.a))



# Ensure the final binary exists and is rebuilt when sources are newer
.PHONY: ensure-built
# Link the final binary
$(NAME): $(OBJS)
	$(call print_status,$(GREEN),LINK,$@)
	@if [ -n "$(LIBFT_A)" ]; then \
		$(CC) $(CFLAGS) $(OPTFLAGS) -o $@ $^ $(LIBFT_A) $(LDFLAGS); \
	else \
		$(call print_status,$(YELLOW),WARN,libft.a not found — linking without libft); \
		$(CC) $(CFLAGS) $(OPTFLAGS) -o $@ $^ $(LDFLAGS); \
	fi

# Compile C source to object files and emit dependency files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(call print_status,$(CYAN),COMPILING,$<)
	@$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@ $(PREPROCFLAGS) $(patsubst %.o,%.d,$@) -I$(SUBMODULE_DIR)/include -I./incs

# Include dependency files if present
-include $(DEPS)

clean:
	$(call print_status,$(RED),CLEAN,Removing objects)
	@rm -rf $(OBJDIR)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing binary)
	$(MAKE) -C libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re