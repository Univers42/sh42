/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:45:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 17:35:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "shell.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>

/* Return the string following -c, or NULL if not provided */
const char *find_command_string(int argc, char **argv)
{
    int i = 1;

    while (i < argc)
    {
        if (ft_strcmp(argv[i], "-c") == 0)
        {
            if (i + 1 < argc)
                return (argv[i + 1]);
            fprintf(stderr, "Error: -c requires a command string argument\n");
            return (NULL);
        }
        i++;
    }
    return (NULL);
}

/* Return 1 if any --debug or --debug=... flag is present (avoid libft here) */
int is_debug_mode(int argc, char **argv)
{
    int i = 1;

    while (i < argc)
    {
        /* matches --debug and --debug=... */
        if (strncmp(argv[i], "--debug", 7) == 0 &&
            (argv[i][7] == '\0' || argv[i][7] == '='))
            return (1);
        i++;
    }
    return (0);
}

/* Execute a single command string (non-interactive). Prints lexer debug block if debug != 0 */
int handle_command_string(const char *command_str, int debug)
{
    t_scanner *scanner;

    if (command_str == NULL)
        return (1);

    scanner = init_scanner(command_str);
    if (debug)
    {
        printf("\n=== LEXER DEBUG OUTPUT ===\n");
        scan_all_tokens(scanner, 1);
        printf("=== END DEBUG OUTPUT ===\n\n");
    }
    else
    {
        scan_all_tokens(scanner, 0);
    }
    return (0);
}
