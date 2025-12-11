/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 19:05:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 19:10:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void    err_cmd_not_found(const char *ctx, const char *cmd)
{
	ft_eprintf("%s: %s: command not found\n", ctx, cmd);
}

void    err_no_path_var(const char *ctx, const char *cmd)
{
	ft_eprintf("%s: %s: No such file or directory\n", ctx, cmd);
}

void	xerrno(const char *ctx, const char *p1)
{
	ft_eprintf("%s: %s: %s\n", ctx, p1, strerror(errno));
}

void	err2(const char *ctx, const char *p1, const char *p2)
{
	ft_eprintf("%s: %s: %s\n", state->context, p1, p2);
}
