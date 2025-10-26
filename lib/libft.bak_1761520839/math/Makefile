# Build orchestration for math submodules (parallel, thread-safe output, colored)

SUBDIRS := geometry algebra

# Number of parallel jobs for sub-makes (default: CPU count)
JOBS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

# ANSI color codes (used in recipe printf)
CYAN   := \033[1;36m
GREEN  := \033[1;32m
YELLOW := \033[1;33m
MAGENTA:= \033[1;35m
RED    := \033[1;31m
RESET  := \033[0m
BOLD   := \033[1m

.PHONY: all $(SUBDIRS) clean fclean re test

all: $(SUBDIRS)

$(SUBDIRS):
	@printf "$(CYAN)==>$(RESET) $(BOLD)Building math/%s$(RESET) (jobs=$(JOBS))\n" "$@"
	@if [ -f "$@/Makefile" ]; then \
		$(MAKE) -C $@ -j $(JOBS) all; \
	else \
		printf "$(YELLOW)Note:$(RESET) No Makefile in %s, skipping\n" "$@"; \
	fi

# Parallel launcher: start sub-makes in background, capture per-subproject logs,
# then wait for each to finish and print its logfile atomically.
test:
	@printf "$(MAGENTA)==> Running tests in math submodules (parallel, thread-safe output)$(RESET)\n"
	@mapfile=$$(mktemp /tmp/math-submap.XXXXXX); \
	for d in $(SUBDIRS); do \
		if [ -f $$d/Makefile ]; then \
			log=$$(mktemp /tmp/math-$(notdir $$d)-XXXXXX.log); \
			printf "$(CYAN)Launching$(RESET) %s -> %s\n" "$$d" "$$log"; \
			( cd $$d && $(MAKE) -j $(JOBS) test ) >$$log 2>&1 & \
			printf "%s %s %s\n" "$$!" "$$log" "$$d" >> $$mapfile; \
		fi; \
	done; \
	ERR=0; \
	if [ -f $$mapfile ]; then \
		while IFS=" " read -r pid logfile dir; do \
			if [ -z "$$pid" ]; then continue; fi; \
			wait $$pid; status=$$?; \
			if [ $$status -eq 0 ]; then \
				printf "\n$(GREEN)==== [%s] %s (status=%d) ====$(RESET)\n" "$$(date +%H:%M:%S)" "$$dir" $$status; \
			else \
				printf "\n$(RED)==== [%s] %s (status=%d) ====$(RESET)\n" "$$(date +%H:%M:%S)" "$$dir" $$status; \
			fi; \
			cat $$logfile || true; \
			rm -f $$logfile; \
			if [ $$status -ne 0 ]; then ERR=1; fi; \
		done < $$mapfile; \
		rm -f $$mapfile; \
	fi; \
	if [ $$ERR -ne 0 ]; then \
		printf "$(RED)One or more sub-make test targets failed$(RESET)\n"; exit 2; \
	else \
		printf "$(GREEN)All subproject tests passed$(RESET)\n"; \
	fi

clean:
	@printf "$(CYAN)==> Cleaning math submodules$(RESET)\n"
	@for d in $(SUBDIRS); do \
		if [ -f $$d/Makefile ]; then \
			printf "$(YELLOW)Cleaning$(RESET) %s\n" "$$d"; \
			$(MAKE) -C $$d clean || true; \
		fi; \
	done

fclean: clean
	@printf "$(CYAN)==> Full clean math submodules$(RESET)\n"
	@for d in $(SUBDIRS); do \
		if [ -f $$d/Makefile ]; then \
			printf "$(YELLOW)Fclean$(RESET) %s\n" "$$d"; \
			$(MAKE) -C $$d fclean || true; \
		fi; \
	done
MAKEFLAGS = --no-print-directory
re: fclean all
