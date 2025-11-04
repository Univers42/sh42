/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:09:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 16:43:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include "config.h"

/* Conditional function declarations */
#if FEATURE_HELP
void print_usage(const char *program_name);
int handle_help_mode(int argc, char **argv);
#endif

#if FEATURE_DEBUG
int handle_debug_mode(int argc, char **argv);
#endif

#if FEATURE_LOGIN
int handle_login_mode(int argc, char **argv);
#endif

#if FEATURE_POSIX
int handle_posix_mode(int argc, char **argv);
#endif

#if FEATURE_PRETTY_PRINT
int handle_pretty_print_mode(int argc, char **argv);
#endif

/* Command string mode (-c flag) */
const char *find_command_string(int argc, char **argv);
int is_debug_mode(int argc, char **argv);
int handle_command_string(const char *command_str, int debug);

#endif /* SHELL_H */
