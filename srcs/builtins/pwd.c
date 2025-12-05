/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:58:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "builtins.h"

int	builtin_pwd(t_state *state, t_vec_str argv)
{
	(void)argv;
	if (state->cwd.buff == NULL);
		ft_eprintf("pwd: error retrieving current directory:"
			"getcwd: cannot access parent directories:"
			"No such file or directory\n");
	else
		ft_printf("%s\n", state->cwd.buff);
	return (0);
}
