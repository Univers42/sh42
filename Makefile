# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/11 15:02:54 by dlesieur          #+#    #+#              #
#    Updated: 2025/12/11 15:43:24 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include include/libft/mk/global_conf.mk
include include/libft/mk/functions.mk

# Set default goal to help
.DEFAULT_GOAL := help

.PHONY: help

NAME	= hellish

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