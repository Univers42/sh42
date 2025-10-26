# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/22 23:00:39 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/23 00:24:03 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEFAULT_SUBDIRS := ctype strings stdio stdlib math memory render sort data_structures debug time

# Config file (optional). If present it may set SUBDIRS.
CONFIG_FILE ?= .libftmodules

# Effective SUBDIRS: user can override by:
#   - editing CONFIG_FILE so it contains "SUBDIRS := a b c"
#   - passing SUBDIRS on the make command line: make SUBDIRS="stdio math" all
SUBDIRS ?= $(DEFAULT_SUBDIRS)
-include $(CONFIG_FILE)

JOBS   ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

MAKEFLAGS += --no-print-directory

.DEFAULT_GOAL := all

.PHONY: all test_all clean fclean re $(SUBDIRS) modules show-config save-config gen-config help libft.a show-libft check_updated

# helper: list available modules (dirs that contain a Makefile)
modules:
	@sh -c 'printf "Available modules (dirs with Makefile):\n"; for d in *; do [ -f "$$d/Makefile" ] && printf "  %s\n" "$$d"; done; exit 0'

# show active configuration
show-config:
	@sh -c 'printf "Config file: %s\n" "$(CONFIG_FILE)"; printf "Active SUBDIRS: %s\n" "$(SUBDIRS)"; printf "Default SUBDIRS: %s\n\n" "$(DEFAULT_SUBDIRS)"; printf "To override for a single invocation: make SUBDIRS=\"stdio math\" all\n"; printf "To persist configuration: make save-config\n"'

# save the current SUBDIRS into CONFIG_FILE
save-config:
	@sh -c 'printf "Saving current SUBDIRS -> %s\n" "$(CONFIG_FILE)"; printf "SUBDIRS := %s\n" "$(SUBDIRS)" > "$(CONFIG_FILE)"; printf "Wrote %s\n" "$(CONFIG_FILE)"'

# generate config from provided variable SUBDIRS_TO_SAVE
# usage: make gen-config [SUBDIRS_TO_SAVE="a b c"]
gen-config:
	@SUBDIRS_TO_USE="$(SUBDIRS_TO_SAVE)"; \
	if [ -z "$$SUBDIRS_TO_USE" ]; then SUBDIRS_TO_USE="$(SUBDIRS)"; fi; \
	printf "SUBDIRS := %s\n" "$$SUBDIRS_TO_USE" > "$(CONFIG_FILE)"; \
	printf "Wrote %s with SUBDIRS: %s\n" "$(CONFIG_FILE)" "$$SUBDIRS_TO_USE"

# small help
help:
	@sh -c 'printf "Usage:\n  make all                # build modules in SUBDIRS (default: all) + libft.a\n  make SUBDIRS=\"stdio math\" all   # build only selected modules + custom libft.a\n  make modules            # list available modules (dirs with Makefile)\n  make show-config        # show current effective configuration\n  make save-config        # persist current SUBDIRS into %s\n  make gen-config SUBDIRS_TO_SAVE=\"a b c\"  # create %s\n  make show-libft         # show object files in libft.a\n  make check_updated      # check if selected modules are up to date\n" "$(CONFIG_FILE)" "$(CONFIG_FILE)"'

# helper: $(call run_in_subdirs, "label", target)
define run_in_subdirs
	@for d in $(SUBDIRS); do \
		if [ -f $$d/Makefile ]; then \
			printf "==> %s: %s\n" "$(1)" "$$d"; \
			$(MAKE) -C $$d -j $(JOBS) $(2) || exit $$?; \
		else \
			printf "==> Skipping %s (no Makefile)\n" "$$d"; \
		fi; \
	done
endef

# Build modules first, then aggregate into libft.a
all: check_updated $(SUBDIRS) libft.a

$(SUBDIRS):
	$(MAKE) -C $@ all

# Create libft.a by collecting .o files from selected modules' obj/ dirs
libft.a: $(SUBDIRS)
	@sh -c 'printf "\033[1;35m\033[1m[LIBFT] Creating libft.a from selected modules...\033[0m\n"; objs=$$(find $(SUBDIRS) -name "*.o" 2>/dev/null); if [ -n "$$objs" ]; then if [ -f libft.a ] && [ "$$(find $(SUBDIRS) -name "*.o" -newer libft.a 2>/dev/null | wc -l)" -eq 0 ]; then printf "\033[1;32m\033[1m[LIBFT] libft.a is up to date\033[0m\n"; else $(AR) $(ARFLAGS) libft.a $$objs >/dev/null 2>&1; printf "\033[1;32m\033[1m[LIBFT] libft.a created successfully\033[0m\n"; fi; else printf "\033[1;31m\033[1m[LIBFT] Failed to create libft.a (no objects found)\033[0m\n"; fi'

# Show contents of libft.a (object files)
show-libft:
	@sh -c 'if [ -f libft.a ]; then printf "\033[1;36m\033[1m[LIBFT] Contents of libft.a:\033[0m\n"; ar -t libft.a | sort; else printf "\033[1;31m\033[1m[LIBFT] libft.a not found\033[0m\n"; fi'

fclean: clean
	$(call print_status,$(RED),FCLEAN,Removing libft.a and cleaning all modules...)
	@$(RM) libft.a
	@for d in $(SUBDIRS); do \
		if [ -f $$d/Makefile ]; then \
			$(MAKE) -C $$d fclean >/dev/null 2>&1; \
		fi; \
	done
	$(call print_status,$(GREEN),FCLEAN,All modules cleaned successfully)

re:
	$(MAKE) fclean
	$(MAKE) all

# Check if selected modules are up to date (rebuilds if needed, reports status)
check_updated:
	@sh -c 'for d in $(SUBDIRS); do if [ -f "$$d/Makefile" ]; then printf "\033[1;36m\033[1m[CHECK] Checking $$d...\033[0m\n"; if $(MAKE) -C $$d -q all >/dev/null 2>&1; then printf "\033[1;32m\033[1m[CHECK] $$d is up to date\033[0m\n"; else printf "\033[1;33m\033[1m[CHECK] $$d needs rebuild\033[0m\n"; fi; fi; done'

norminette:
	@output="$$(find . \( -path "./minilibx-linux" -o -path "./testing" \) -prune -o -name "*.c" -exec norminette {} + 2>/dev/null)"; \
	if echo "$$output" | grep -q "Error"; then \
		echo "$$output" | grep "Error"; \
		echo "❌ Norminette errors detected!"; \
	else \
		echo "✅ All files passed norminette!"; \
	fi