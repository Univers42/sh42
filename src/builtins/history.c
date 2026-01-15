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
#include <readline/readline.h>

/*
** Display history entries.
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
** Rewrite the entire history file from our vector.
** This is called after delete/clear to sync the file.
*/
static void	rewrite_history_file(t_shell *state)
{
	char	*hist_path;
	int		fd;
	size_t	i;
	char	**arr;

	/* Close existing append fd */
	if (state->hist.append_fd >= 0)
	{
		close(state->hist.append_fd);
		state->hist.append_fd = -1;
	}
	hist_path = get_hist_file_path(state);
	if (!hist_path)
		return ;
	/* Truncate and rewrite */
	fd = open(hist_path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd >= 0)
	{
		arr = (char **)state->hist.hist_cmds.ctx;
		i = 0;
		while (i < state->hist.hist_cmds.len)
		{
			t_string enc = encode_cmd_hist(arr[i]);
			if (enc.ctx)
			{
				write_to_file((char *)enc.ctx, fd);
				free(enc.ctx);
			}
			i++;
		}
		close(fd);
	}
	/* Reopen for append */
	state->hist.append_fd = open(hist_path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	free(hist_path);
}

/*
** Clear all history entries and truncate the history file.
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
	/* Clear readline's internal history */
	rl_clear_history();
	/* Rewrite (truncate) the history file */
	rewrite_history_file(state);
	return (0);
}

/*
** Delete a specific history entry by 1-based index.
** Returns 0 on success, 1 on error.
*/
static int	history_delete(t_shell *state, char *arg)
{
	int			idx;
	size_t		pos;
	size_t		i;
	char		**arr;
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
	/* Remove from readline's history FIRST (before modifying our vector) */
	entry = remove_history((int)pos);
	if (entry)
	{
		/* readline allocated entry->line, we must free it */
		free(entry->line);
		free(entry);
	}
	/* Now free the entry from our vector */
	arr = (char **)state->hist.hist_cmds.ctx;
	free(arr[pos]);
	/* Shift array left */
	i = pos;
	while (i + 1 < state->hist.hist_cmds.len)
	{
		arr[i] = arr[i + 1];
		i++;
	}
	state->hist.hist_cmds.len--;
	/* Rewrite the history file to reflect the deletion */
	rewrite_history_file(state);
	return (0);
}

/*
** Main history builtin entry point.
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
			history_clear(state);
		else if (ft_strncmp(arg, "-d", 2) == 0)
		{
			if (arg[2] != '\0')
			{
				if (history_delete(state, arg + 2))
					return (1);
			}
			else if (i + 1 >= argv.len)
			{
				ft_eprintf("%s: history: -d: option requires an argument\n",
					state->context);
				return (1);
			}
			else
			{
				i++;
				if (history_delete(state, ((char **)argv.ctx)[i]))
					return (1);
			}
		}
		else if (arg[0] == '-' && arg[1] != '\0' && !ft_isdigit(arg[1]))
		{
			ft_eprintf("%s: history: %s: invalid option\n",
				state->context, arg);
			return (2);
		}
		else
		{
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
