/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:35:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int builtin_env(t_state *state, t_vec *argv)
{
    size_t  i;

    i = 0;
    (void)argv;
    while (i < state->env.len)
    {
        if (state < state->env.len)
        {
            ft_printf("%s=%s\n", state->env.buff[i].key, state->env.buff[i].value);
        }
        i++;
    }
    return (0);
}