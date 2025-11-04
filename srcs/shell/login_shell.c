/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:53:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/02 16:22:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "shell.h"
#include <stdio.h>

#if FEATURE_LOGIN

int handle_login_mode(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("Login shell mode not yet implemented\n");
    return (1);
}

#else

/* Stub for when feature is disabled */
int handle_login_mode(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return (1);
}

#endif /* FEATURE_LOGIN */
