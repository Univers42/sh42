/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging_mode.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 16:23:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "shell.h"
#include "lexer.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if FEATURE_DEBUG

static int starts_with(const char *s, const char *pre)
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

static const char *extract_debug_mode(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
        if (starts_with(argv[i], "--debug"))
        {
            if (argv[i][7] == '=')
                return argv[i] + 8; /* after '=' */
            return NULL;            /* plain --debug */
        }
        i++;
    }
    return NULL;
}

static void print_debug_help(void)
{
    printf("Debug modes available:\n");
    printf("  --debug          Enable all debug output\n");
    printf("  --debug=lexer    Enable lexer debug output only\n");
    printf("  --debug=parser   Enable parser debug output only\n");
    printf("  --debug=exec     Enable executor debug output only\n");
}

int handle_debug_mode(int argc, char **argv)
{
    bool run;
    char *line;
    t_scanner *scanner;
    const char *mode;
    bool debug_lexer;

    mode = extract_debug_mode(argc, argv);
    debug_lexer = true;

    if (mode)
    {
        if (ft_strcmp(mode, "lexer") == 0)
            debug_lexer = true;
        else if (ft_strcmp(mode, "parser") == 0)
            debug_lexer = false;
        else if (ft_strcmp(mode, "help") == 0)
        {
            print_debug_help();
            return (0);
        }
        else
        {
            printf("Unknown debug mode: %s\n", mode);
            print_debug_help();
            return (1);
        }
    }

    run = true;
    printf("[DEBUG MODE ACTIVE");
    if (mode)
        printf(" - %s", mode);
    printf("]\n");

    while (run)
    {
        line = readline("[DEBUG] $> ");
        if (line == NULL)
            break;
        if (line[0] != '\0')
            add_history(line);

        if (debug_lexer)
        {
            printf("\n=== LEXER DEBUG OUTPUT ===\n");
            scanner = init_scanner(line);
            scan_all_tokens(scanner, 1);
            printf("=== END DEBUG OUTPUT ===\n\n");
        }
        else
        {
            scanner = init_scanner(line);
            scan_all_tokens(scanner, 0);
        }

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

#endif /* FEATURE_DEBUG */