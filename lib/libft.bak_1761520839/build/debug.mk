# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    debug.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 15:22:54 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/26 15:23:06 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                            DEBUG TARGETS                                ║
# ════════════════════════════════════════════════════════════════════════════

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(THIS_DIR)common.mk

# Debug flags
DEBUG_FLAGS = -g3 -DDEBUG
SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined
OPTIMIZE_FLAGS = -O0
PROJECT			:= $(pwd)
# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS) $(OPTIMIZE_FLAGS)
debug: all
	$(call print_status,$(YELLOW),DEBUG,Debug mode enabled)

# Debug with sanitizers
.PHONY: debug_sanitize
debug_sanitize: CFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS) $(OPTIMIZE_FLAGS)
debug_sanitize: all
	$(call print_status,$(YELLOW),DEBUG,Debug mode with sanitizers enabled)
# $(call logging,STATE_TOKEN,Message)
# Produces: configure [STATE] : Message (colorized)
define logging
	@printf "%s%s%s[%s%s%s]%s: %s%s\n" \
		"$(FADED_BOLD_GRAY)" "$(notdir $(CURDIR))" "$(FADED_BOLD_GRAY)" \
		"$(1)" "$(2)" "$(FADED_BOLD_GRAY)" \
		"$(FADED_BOLD_GRAY)" "$(WHITE)$(3)" "$(RESET)"
endef

define log_info
	$(call logging,$(CYAN),INFO,$(1))
endef

define log_warn
	$(call logging,$(YELLOW),WARN,$(1))
endef

define log_ok
	$(call logging,$(GREEN),OK,$(1))
endef

define log_note
	$(call logging,$(BLUE),NOTE,$(1))
endef

define log_error
	$(call logging,$(RED),ERROR,$(1))
endef

define log_debug
	$(call logging,$(MAGENTA),DEBUG,$(1))
endef

# Release build
.PHONY: release
release: CFLAGS += -O3 -DNDEBUG
release: all
	$(call print_status,$(GREEN),RELEASE,Release mode enabled)

# Profile build
.PHONY: profile
profile: CFLAGS += -pg -O2
profile: all
	$(call print_status,$(BLUE),PROFILE,Profile mode enabled)

# Static analysis
.PHONY: analyze
analyze:
	$(call print_status,$(CYAN),ANALYZE,Running static analysis...)
	@cppcheck --enable=all --std=c99 $(SRC_DIRS) 2>/dev/null || true
	@scan-build make fclean all 2>/dev/null || true
