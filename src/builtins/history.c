/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 12:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/01/15 12:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "env.h"
#include <readline/history.h>
# include <readline/readline.h>

/*
** Display history entries.
** If count <= 0 or count > total, show all entries.
** Otherwise show only the last 'count' entries.
** Entries are numbered starting from 1.
*/
static int	history_display(t_shell *state, int count)
{
	size_t	total;
	size_t	start;
	size_t	i;
	int		num;

	total = state->hist.hist_cmds.len;
	if (count <= 0 || (size_t)count > total)
		start = 0;
	else
		start = total - (size_t)count;
	i = start;
	while (i < total)
	{
		num = (int)(i + 1);
		ft_printf("%5d  %s\n", num, ((char **)state->hist.hist_cmds.ctx)[i]);
		i++;
	}
	return (0);
}

/*
** Clear all history entries.
** Frees memory and clears readline's internal history.
*/
static int	history_clear(t_shell *state)
{
	size_t	i;

	i = 0;
	while (i < state->hist.hist_cmds.len)
	{
		free(((char **)state->hist.hist_cmds.ctx)[i]);
		i++;
	}
	state->hist.hist_cmds.len = 0;
	/* Also clear readline's internal history */
	rl_clear_history();
	return (0);
}

/*
** Delete a specific history entry by 1-based index.
** Returns 0 on success, 1 on error.
*/
static int	history_delete(t_shell *state, char *arg)
{
	int		idx;
	size_t	pos;
	size_t	i;
	char	**arr;
	HIST_ENTRY	*entry;

	if (ft_checked_atoi(arg, &idx, 0))
	{
		ft_eprintf("%s: history: %s: numeric argument required\n",
			state->context, arg);
		return (1);
	}
	if (idx < 1)
	{
		ft_eprintf("%s: history: %d: history position out of range\n",
			state->context, idx);
		return (1);
	}
	pos = (size_t)(idx - 1);
	if (pos >= state->hist.hist_cmds.len)
	{
		ft_eprintf("%s: history: %d: history position out of range\n",
			state->context, idx);
		return (1);
	}
	/* Free the entry and shift array */
	arr = (char **)state->hist.hist_cmds.ctx;
	free(arr[pos]);
	i = pos;
	while (i + 1 < state->hist.hist_cmds.len)
	{
		arr[i] = arr[i + 1];
		i++;
	}
	state->hist.hist_cmds.len--;
	/* Also remove from readline's history (0-indexed) */
	entry = remove_history((int)pos);
	if (entry)
	{
		free(entry->line);
		free(entry);
	}
	return (0);
}

/*
** Parse and validate the -d option argument.
** Returns the next argument index to process, or -1 on error.
*/
static int	parse_delete_option(t_shell *state, t_vec argv, size_t *i)
{
	char	*arg;

	arg = ((char **)argv.ctx)[*i];
	/* Check if -d is followed by number in same arg (e.g., "-d5") */
	if (arg[2] != '\0')
	{
		if (history_delete(state, arg + 2))
			return (-1);
		return (0);
	}
	/* Number should be next argument */
	if (*i + 1 >= argv.len)
	{
		ft_eprintf("%s: history: -d: option requires an argument\n",
			state->context);
		return (-1);
	}
	(*i)++;
	if (history_delete(state, ((char **)argv.ctx)[*i]))
		return (-1);
	return (0);
}

/*
** Main history builtin entry point.
** Supports: history, history N, history -c, history -d N
*/
int	builtin_history(t_shell *state, t_vec argv)
{
	size_t	i;
	char	*arg;
	int		count;

	if (!state->hist.hist_active)
	{
		ft_eprintf("%s: history: history is not active\n", state->context);
		return (1);
	}
	if (argv.len == 1)
		return (history_display(state, -1));
	i = 1;
	while (i < argv.len)
	{
		arg = ((char **)argv.ctx)[i];
		if (ft_strcmp(arg, "-c") == 0)
		{
			history_clear(state);
		}
		else if (ft_strncmp(arg, "-d", 2) == 0)
		{
			if (parse_delete_option(state, argv, &i) < 0)
				return (1);
		}
		else if (arg[0] == '-' && arg[1] != '\0' && !ft_isdigit(arg[1]))
		{
			ft_eprintf("%s: history: %s: invalid option\n",
				state->context, arg);
			return (2);
		}
		else
		{
			/* Treat as count argument */
			if (ft_checked_atoi(arg, &count, 0))
			{
				ft_eprintf("%s: history: %s: numeric argument required\n",
					state->context, arg);
				return (1);
			}
			return (history_display(state, count));
		}
		i++;
	}
	return (0);
}
