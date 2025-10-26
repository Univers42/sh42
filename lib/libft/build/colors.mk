# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    colors.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 19:30:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/26 15:23:28 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                            COLOR DEFINITIONS                             ║
# ════════════════════════════════════════════════════════════════════════════

# Use a real escape character so printf will emit colors instead of the literal "\033"
ESC := $(shell printf '\033')

#BLACK = $(ESC)[0;30m
#RED = $(ESC)[0;31m
#GREEN = $(ESC)[0;32m
#YELLOW = $(ESC)[0;33m
#BLUE = $(ESC)[0;34m
#MAGENTA = $(ESC)[0;35m
#CYAN = $(ESC)[0;36m
#WHITE = $(ESC)[0;37m
#
## Bold colors
#BOLD_BLACK = $(ESC)[1;30m
#BOLD_RED = $(ESC)[1;31m
#BOLD_GREEN = $(ESC)[1;32m
#BOLD_YELLOW = $(ESC)[1;33m
#BOLD_BLUE = $(ESC)[1;34m
#BOLD_MAGENTA = $(ESC)[1;35m
#BOLD_CYAN = $(ESC)[1;36m
#BOLD_WHITE = $(ESC)[1;37m
#
## Background colors
#BG_BLACK = $(ESC)[40m
#BG_RED = $(ESC)[41m
#BG_GREEN = $(ESC)[42m
#BG_YELLOW = $(ESC)[43m
#BG_BLUE = $(ESC)[44m
#BG_MAGENTA = $(ESC)[45m
#BG_CYAN = $(ESC)[46m
#BG_WHITE = $(ESC)[47m
#
## Special formatting
#RESET = $(ESC)[0m
#BOLD = $(ESC)[1m
#DIM = $(ESC)[2m
#ITALIC = $(ESC)[3m
#UNDERLINE = $(ESC)[4m
#BLINK = $(ESC)[5m
#REVERSE = $(ESC)[7m
#STRIKETHROUGH = $(ESC)[9m
#
## Enhanced colors and formatting
#BRIGHT_GREEN = $(ESC)[1;32m
#BRIGHT_RED = $(ESC)[1;31m
#BRIGHT_YELLOW = $(ESC)[1;33m
#BRIGHT_BLUE = $(ESC)[1;34m
#BRIGHT_MAGENTA = $(ESC)[1;35m
#BRIGHT_CYAN = $(ESC)[1;36m
#
## Faded bold gray (bold + dim + white)
#FADED_BOLD_GRAY = $(ESC)[1;2;37m

BLACK        := $(shell printf '\033[0;30m')
RED          := $(shell printf '\033[0;31m')
GREEN        := $(shell printf '\033[0;32m')
YELLOW       := $(shell printf '\033[0;33m')
BLUE         := $(shell printf '\033[0;34m')
MAGENTA      := $(shell printf '\033[0;35m')
CYAN         := $(shell printf '\033[0;36m')
WHITE        := $(shell printf '\033[0;37m')

# Bold colors
BOLD_BLACK   := $(shell printf '\033[1;30m')
BOLD_RED     := $(shell printf '\033[1;31m')
BOLD_GREEN   := $(shell printf '\033[1;32m')
BOLD_YELLOW  := $(shell printf '\033[1;33m')
BOLD_BLUE    := $(shell printf '\033[1;34m')
BOLD_MAGENTA := $(shell printf '\033[1;35m')
BOLD_CYAN    := $(shell printf '\033[1;36m')
BOLD_WHITE   := $(shell printf '\033[1;37m')

# Background colors
BG_BLACK     := $(shell printf '\033[40m')
BG_RED       := $(shell printf '\033[41m')
BG_GREEN     := $(shell printf '\033[42m')
BG_YELLOW    := $(shell printf '\033[43m')
BG_BLUE      := $(shell printf '\033[44m')
BG_MAGENTA   := $(shell printf '\033[45m')
BG_CYAN      := $(shell printf '\033[46m')
BG_WHITE     := $(shell printf '\033[47m')

# Special formatting
RESET        := $(shell printf '\033[0m')
BOLD         := $(shell printf '\033[1m')
DIM          := $(shell printf '\033[2m')
ITALIC       := $(shell printf '\033[3m')
UNDERLINE    := $(shell printf '\033[4m')
BLINK        := $(shell printf '\033[5m')
REVERSE      := $(shell printf '\033[7m')
STRIKETHROUGH:= $(shell printf '\033[9m')

# Enhanced colors and formatting
BRIGHT_GREEN := $(shell printf '\033[1;32m')
BRIGHT_RED   := $(shell printf '\033[1;31m')
BRIGHT_YELLOW:= $(shell printf '\033[1;33m')
BRIGHT_BLUE  := $(shell printf '\033[1;34m')
BRIGHT_MAGENTA:= $(shell printf '\033[1;35m')
BRIGHT_CYAN  := $(shell printf '\033[1;36m')

# State-specific colors (used by logging macro)
STATE_COLOR_INFO  := $(BRIGHT_CYAN)
STATE_COLOR_WARN  := $(BRIGHT_YELLOW)
STATE_COLOR_OK    := $(BRIGHT_GREEN)
STATE_COLOR_NOTE  := $(BRIGHT_MAGENTA)
STATE_COLOR_ERROR := $(BRIGHT_RED)
STATE_COLOR_DEBUG := $(BRIGHT_BLUE)