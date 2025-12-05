/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:56 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 17:18:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "builtins.h"

# ifndef HAS_VAR
void    try_unset(t_state *state, char *key)
{
	t_env	*e;

	e = env_get(&state->env, key);
	if (e)
		vec_env_del(&state->env, e - state->env.buff);
}

int builtin_unset(t_hellish *state, t_vec argv)
{
	size_t  i;

	i = 1;
	while (i < argv.len)
	{
		try_unset(state, argv.buf[i]);
		i++;
	}
	return (0);
}
#else

#endif