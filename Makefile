# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/11 15:02:54 by dlesieur          #+#    #+#              #
#    Updated: 2025/12/11 21:13:46 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include include/libft/mk/global_conf.mk
include include/libft/mk/functions.mk

.PHONY: help all clean fclean re fre ffclean submodules progress_init

NAME	= hellish
LIBDPDC := libft.a

SOURCES := $(shell find srcs -type f -name '*.c')
OBJS	:= $(SOURCES:.c=.o)
TOTAL_OBJS := $(words $(OBJS))

RM ?= rm -f

PROGRESS_WIDTH ?= 40
COUNTER_FILE := .build_count
SPIN_FILE := .spin_state

# Unicode spinner frames (smooth Braille animation)
SPINNER_FRAMES := ⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏

# Spinner script
define SPINNER_SCRIPT
	trap "exit 0" SIGTERM
	frames="$(SPINNER_FRAMES)"
	i=0
	while true; do
		frame=$$(echo "$$frames" | cut -d' ' -f$$((i % 10 + 1)))
		printf "\r$(BRIGHT_CYAN)  %s$(RESET)  $(DIM)Compiling project...$(RESET)" "$$frame"
		i=$$((i + 1))
		sleep 0.1
	done
endef
export SPINNER_SCRIPT

ifeq ($(strip $(SUBMODULE_DIRS)),)
SUBMODULE_TARGETS :=
else
SUBMODULE_TARGETS := submodules
endif

# Wrapper target
all: progress_init
	@if [ ! -f $(SPIN_FILE) ]; then \
		bash -c "$$SPINNER_SCRIPT" & echo $$! > $(SPIN_FILE); \
	fi
	@$(MAKE) $(NAME) --no-print-directory
	@if [ -f $(SPIN_FILE) ]; then \
		kill $$(cat $(SPIN_FILE)) 2>/dev/null; \
		rm -f $(SPIN_FILE); \
		printf "\r$(BRIGHT_GREEN)  ✓$(RESET)  $(DIM)Project built successfully$(RESET)      \n"; \
	fi

progress_init:
	@rm -f $(COUNTER_FILE) $(SPIN_FILE)
	@$(call log_info,🚀 Starting build of $(NAME))

$(NAME): $(SUBMODULE_TARGETS) $(LIBDPDC) $(OBJS)
	@printf "\r$(BRIGHT_CYAN)  🔗 Linking$(RESET) $(BRIGHT_CYAN)%s$(RESET)            \n" "$(NAME)"
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) include/libft/libft.a 2>/dev/null
	@printf "$(BRIGHT_GREEN)  ✓ %s$(RESET) $(BRIGHT_GREEN)ready$(RESET)\n" "$(NAME)"

$(LIBDPDC): 
	@$(MAKE) -C include/libft all -j$$(nproc) > /dev/null
	
submodules:
	@echo "Building submodules..."
	@for d in $(SUBMODULE_DIRS); do \
		[ -d "$$d" ] || { echo "warning: submodule dir $$d not found"; continue; }; \
		$(MAKE) -C $$d || exit 1; \
	done

# Silent compilation
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(COUNTER_FILE) $(SPIN_FILE)

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C include/libft fclean

ffclean: fclean
ifneq ($(strip $(SUBMODULE_DIRS)),)
	@for d in $(SUBMODULE_DIRS); do \
		[ -d "$$d" ] && $(MAKE) -C $$d ffclean || true; \
	done
endif

fre: ffclean all

re: fclean all

help:
	@echo ""
	@$(call log_title,              sh42 Makefile Targets)
	@echo ""
	@$(call log_info,📦 Build Targets:)
	@$(call log_ok,make or make all - Build the minishell binary (default target))
	@$(call log_ok,make re - Clean and rebuild from scratch)
	@$(call log_ok,make fre - Full clean (includes libft) and rebuild)
	@echo ""
	@$(call log_info,🧹 Clean Targets:)
	@$(call log_ok,make clean - Remove object files and archives)
	@$(call log_ok,make fclean - Remove binary and object files)
	@$(call log_ok,make ffclean - Full clean (also cleans libft submodule))
	@echo ""
	@$(call log_info,⚙️  Build Configuration Targets:)
	@$(call log_ok,make help-only - Build with FEATURE_HELP_ONLY)
	@$(call log_ok,make debug-only - Build with FEATURE_DEBUG_ONLY)
	@$(call log_ok,make all-features - Build with all features enabled)
	@$(call log_ok,make minimal - Build minimal core (no optional features))
	@echo ""
	@$(call log_info,🧪 Testing Targets:)
	@$(call log_ok,make test-lexer - Run lexer tests)
	@$(call log_ok,make test-lexer-verbose - Run lexer tests with verbose output)
	@$(call log_ok,make test-lexer-raw - Run raw lexer tests (prints to stdout))
	@echo ""
	@$(call log_info,🔀 Git/Push Targets:)
	@$(call log_ok,make push_home - Push to home remote (requires MSG="..."))
	@$(call log_ok,make push_campus - Push all to campus remote (requires MSG="..."))
	@$(call log_ok,make publish - Publish feature branch (requires MSG="..."))
	@$(call log_ok,make finish - Finish feature branch workflow)
	@$(call log_ok,make start_feat - Start a new feature branch (requires BRANCH="..."))
	@$(call log_ok,make start_fix - Start a bugfix branch (requires BRANCH="..."))
	@$(call log_ok,make start_release - Start a release branch (requires VERSION="..."))
	@echo ""
	@$(call log_info,🔧 Maintenance Targets:)
	@$(call log_ok,make configure - Set up git hooks and scripts)
	@$(call log_ok,make update - Update libft submodule to latest)
	@echo ""
	@$(call log_note,📝 Examples:)
	@$(call log_ok,make - Build the project)
	@$(call log_ok,make re - Clean and rebuild)
	@$(call log_ok,make publish MSG="feat: add feature" - Publish feature branch)
	@$(call log_ok,make test-lexer-verbose ID=L1 - Run specific lexer test)
	@echo ""