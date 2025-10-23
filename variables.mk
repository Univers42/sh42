# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    variables.mk                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/23 19:04:34 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/23 19:06:08 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	shell
DPDCS			:=	libft.a
# Language standard and feature macros
STD				:=	-std=gnu99
DEFINES			:=	-D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L

# Warning and hardening flags (keep strictness)
WARN_FLAGS		:=	-Wall -Wextra -Werror -pedantic -fmax-errors=5 \
					-Wshadow -Wpointer-arith -Wcast-align -Wvolatile-register-var \
					-Wdangling-else -Wenum-compare -Wexpansion-to-defined -Waddress \
					-Wno-shift-count-overflow -Wstrict-overflow -Wfatal-errors

# Analyzer / sanitizers (kept as a dedicated variable)
ANAFLAGS		:=	-fanalyzer -fsanitize=address,undefined,leak

# Preprocessor flags
PREPROCFLAGS	:=	-dF -dI -dN -dU -M -MD -MF -MG -MMD -MP -MQ -MT
CPPFLAGS		:=	$(DEFINES)

# Debug flags (overridable) and optimization flags
DEBFLAGS		?=	-g3 -O0 -ggdb
OPTFLAGS		:=	-Ofast -faggressive-loop-optimizations -pipe

# Final CFLAGS composition (keeps strict warnings and optional analyzers)
CFLAGS			:=	$(STD) $(WARN_FLAGS) $(DEBFLAGS) $(CPPFLAGS) $(PREPROCFLAGS) $(ANAFLAGS)

# Linker flags
LDFLAGS			:=	-lreadline -pthread -lm

