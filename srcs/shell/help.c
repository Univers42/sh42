/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:13:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 17:35:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "config.h"

#if FEATURE_HELP

void print_usage(const char *program_name)
{
	printf("Usage: %s [OPTIONS]\n\n", program_name);
	printf("Options:\n");

#if FEATURE_HELP
	printf("  --help          Show this help message and exit\n");
#endif
#if FEATURE_DEBUG
	printf("  --debug[=lexer|parser|exec]  Enable debug mode (optional scope)\n");
#endif
	/* New: -c command string (non-interactive) */
	printf("  -c <string>     Execute command string (pairs well with --debug=lexer)\n");
#if FEATURE_LOGIN
	printf("  --login         Run as login shell (reads /etc/profile)\n");
#endif
#if FEATURE_POSIX
	printf("  --posix         Enable strict POSIX compliance mode\n");
#endif
#if FEATURE_PRETTY_PRINT
	printf("  --pretty-print  Display parse tree in readable format\n");
#endif
	printf("\n");
}

int handle_help_mode(int argc, char **argv)
{
	(void)argc;
	print_usage(argv[0]);
	return (0);
}

#endif /* FEATURE_HELP */
