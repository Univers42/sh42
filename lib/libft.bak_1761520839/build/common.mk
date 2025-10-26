# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    common.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 19:30:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/24 18:39:06 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                           COMMON VARIABLES                               ║
# ════════════════════════════════════════════════════════════════════════════

# Compiler and flags (allow overriding from sub-Makefiles)
CC ?= cc
CFLAGS ?= -Wall -Wextra -Werror -MMD -g -O -fPIC
CFLAGS_SHARED ?= -Wall -Werror -Wextra -Wnewline-eof -shared
OUT ?= libft.so
LDFLAGS ?= -L. -lft -pthread -lm
AR ?= ar
ARFLAGS ?= rcs
RM ?= rm -f

# Directories
OBJ_DIR = obj
BUILD_DIR = build

# Colors and formatting
# Determine the absolute directory where this makefile (common.mk) resides
# and include colors.mk from that directory. This makes the include work
# even when common.mk is included from subdirectories.
THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(THIS_DIR)colors.mk

# Logging state tokens and prefix
LOG_PREFIX ?= $(NAME)
STATE_INFO   = INFO
STATE_WARN   = WARN
STATE_OK     = OK
STATE_NOTE   = NOTE
STATE_ERROR  = ERROR
STATE_DEBUG  = DEBUG

# ════════════════════════════════════════════════════════════════════════════
# ║                           COMMON FUNCTIONS                               ║
# ════════════════════════════════════════════════════════════════════════════

# Function to create directory structure
define create_dirs
	@mkdir -p $(OBJ_DIR)
	# NOTE: do not create $(BUILD_DIR) automatically to avoid polluting the repo
	# If you really need build/ for tools, create it manually or via an explicit target.
endef

# Function to print status messages
define print_status
	@printf '%s%s%s[%s%s%s]%s: %s%s\n' \
		"$(FADED_BOLD_GRAY)" "$(notdir $(CURDIR))" "$(FADED_BOLD_GRAY)" \
		"$(1)" "$(2)" "$(FADED_BOLD_GRAY)" \
		"$(FADED_BOLD_GRAY)" "$(WHITE)$(3)" "$(RESET)"
endef

# Function to clean object files (use rm -rf to remove directory reliably)
define clean_objects
	$(call print_status,$(RED),CLEAN,Removing object files...)
	@rm -rf $(OBJ_DIR)
endef

# Function to clean library
define clean_library
	$(call print_status,$(RED),CLEAN,Removing library $(NAME)...)
	@$(RM) $(NAME)
endef
