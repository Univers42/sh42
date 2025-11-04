/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pretty_print_mode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 16:22:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "shell.h"
#include <stdio.h>

#if FEATURE_PRETTY_PRINT

int handle_pretty_print_mode(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("Pretty-print mode not yet implemented\n");
    return (1);
}

#else

/* Stub for when feature is disabled */
int handle_pretty_print_mode(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return (1);
}

#endif /* FEATURE_PRETTY_PRINT */
