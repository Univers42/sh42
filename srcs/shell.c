/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:43:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 16:43:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "lexer.h"
#include "config.h"
#include <string.h> // add this

/* Compile-time feature check - this will cause a warning if not defined */
#if !defined(FEATURE_DEBUG)
#error "FEATURE_DEBUG is not defined! Check config.h inclusion."
#endif

#if !FEATURE_DEBUG
#warning "FEATURE_DEBUG is disabled (value is 0)"
#endif

/* Forward declarations removed - now in shell.h */

/* local, libft-free prefix matcher */
static int has_prefix(const char *s, const char *pre)
{
	size_t i = 0;
	while (pre[i])
	{
		if (s[i] != pre[i])
			return 0;
		i++;
	}
	return 1;
}

static int parse_arguments(int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		fprintf(stderr, "[TRACE] Checking arg[%d]: '%s'\n", i, argv[i]);
		fprintf(stderr, "[TRACE] FEATURE_DEBUG = %d\n", FEATURE_DEBUG);

#if FEATURE_DEBUG
		/* Match --debug or --debug=... without relying on libft */
		if (has_prefix(argv[i], "--debug") && (argv[i][7] == '\0' || argv[i][7] == '='))
		{
			fprintf(stderr, "[TRACE] Matched --debug, calling handler\n");
			return (handle_debug_mode(argc, argv));
		}
#else
		fprintf(stderr, "[TRACE] FEATURE_DEBUG is disabled at compile time\n");
#endif
#if FEATURE_HELP
		if (ft_strcmp(argv[i], "--help") == 0)
		{
			fprintf(stderr, "[TRACE] Matched --help\n");
			return (handle_help_mode(argc, argv));
		}
#endif
#if FEATURE_LOGIN
		if (ft_strcmp(argv[i], "--login") == 0)
		{
			fprintf(stderr, "[TRACE] Matched --login\n");
			return (handle_login_mode(argc, argv));
		}
#endif
#if FEATURE_POSIX
		if (ft_strcmp(argv[i], "--posix") == 0)
		{
			fprintf(stderr, "[TRACE] Matched --posix\n");
			return (handle_posix_mode(argc, argv));
		}
#endif
#if FEATURE_PRETTY_PRINT
		if (ft_strcmp(argv[i], "--pretty-print") == 0)
		{
			fprintf(stderr, "[TRACE] Matched --pretty-print\n");
			return (handle_pretty_print_mode(argc, argv));
		}
#endif
		fprintf(stderr, "[TRACE] Unknown option: %s\n", argv[i]);
		printf("Unknown option: %s\n", argv[i]);
#if FEATURE_HELP
		print_usage(argv[0]);
#endif
		return (1);
		i++;
	}
	return (-1); /* No special mode, continue to normal shell */
}

int main(int argc, char **argv)
{
	int mode_result;
	const char *cmd_str;

	/* Check for -c flag first (command string mode) */
	cmd_str = find_command_string(argc, argv);
	if (cmd_str)
	{
		/* Execute command string and exit */
		return (handle_command_string(cmd_str, is_debug_mode(argc, argv)));
	}

	/* Check for special modes */
	if (argc > 1)
	{
		mode_result = parse_arguments(argc, argv);
		if (mode_result >= 0)
			return (mode_result);
	}

	/* Normal shell mode (no debug output) */
	bool run;
	char *line;
	t_scanner *scanner;

	run = true;
	while (run)
	{
		line = readline("$>");
		if (line == NULL)
			break;
		if (line[0] != '\0')
			add_history(line);
		scanner = init_scanner(line);
		scan_all_tokens(scanner, 0);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		free(line);
	}
	rl_clear_history();
	return (0);
}
