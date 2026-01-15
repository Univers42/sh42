/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 23:33:41 by marvin            #+#    #+#             */
/*   Updated: 2026/01/09 23:33:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <unistd.h>
#include "helpers.h"
#include "env.h"

t_string	parse_single_cmd(t_string hist, size_t *cur)
{
	t_string	cmd;
	bool		bs;
	char		c;

	vec_init(&cmd);
	cmd.elem_size = 1;
	bs = false;
	while (*cur < hist.len)
	{
		c = ((char *)hist.ctx)[*cur];
		if (c == '\\' && !bs)
		{
			bs = true;
			(*cur)++;
			continue ;
		}
		if (c == '\n' && !bs)
		{
			(*cur)++;
			break ;
		}
		vec_push(&cmd, &c);
		bs = false;
		(*cur)++;
	}
	if (!vec_ensure_space_n(&cmd, 1))
		return (cmd);
	((char *)cmd.ctx)[cmd.len] = '\0';
	return (cmd);
}

t_vec	parse_hist_file(t_string hist)
{
	size_t		cur;
	t_vec		ret;
	char		*cmd;

	cur = 0;
	vec_init(&ret);
	ret.elem_size = sizeof(char *);
	while (cur < hist.len)
	{
		cmd = (char *)parse_single_cmd(hist, &cur).ctx;
		if (cmd && cmd[0] != '\0')
		{
			vec_push(&ret, &cmd);
			add_history(cmd);
		}
		else
			free(cmd);
	}
	return (ret);
}

void	parse_history_file(t_shell *state)
{
	t_string	hist;
	int			fd;
	char		*hist_file_path;

	hist_file_path = get_hist_file_path(state);
	if (!hist_file_path)
		return ;
	fd = open(hist_file_path, O_RDONLY | O_CREAT, 0666);
	if (fd < 0)
	{
		warning_error("Can't open the history file for reading");
		free(hist_file_path);
		return ;
	}
	vec_init(&hist);
	hist.elem_size = 1;
	vec_append_fd(fd, &hist);
	close(fd);
	state->hist.hist_cmds = parse_hist_file(hist);
	state->hist.append_fd
		= open(hist_file_path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (state->hist.append_fd < 0)
		warning_error("Can't open the history file for writing");
	free(hist_file_path);
	free(hist.ctx);
}

t_string	encode_cmd_hist(char *cmd)
{
	t_string	ret;

	vec_init(&ret);
	ret.elem_size = 1;
	while (*cmd)
	{
		if (*cmd == '\\')
		{
			char tmp = '\\';
			vec_push(&ret, &tmp);
		}
		if (*cmd == '\n')
		{
			char tmp = '\\';
			vec_push(&ret, &tmp);
		}
		{
			char tmp = *cmd;
			vec_push(&ret, &tmp);
		}
		cmd++;
	}
	{
		char tmp = '\n';
		vec_push(&ret, &tmp);
	}
	if (!vec_ensure_space_n(&ret, 1))
		return (ret);
	((char *)ret.ctx)[ret.len] = '\0';
	return (ret);
}

/*
** Check if the current input line is a duplicate of the last history entry.
** Returns true if it's a duplicate (should not be added again).
*/
static bool	is_duplicate_entry(t_shell *state, const char *entry, size_t len)
{
	char	*last;

	if (state->hist.hist_cmds.len == 0)
		return (false);
	last = ((char **)state->hist.hist_cmds.ctx)[state->hist.hist_cmds.len - 1];
	if (!last)
		return (false);
	return (str_slice_eq_str((char *)entry, len, last));
}

/*
** Check if entry consists only of whitespace.
*/
static bool	is_whitespace_only(const char *s, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
			return (false);
		i++;
	}
	return (true);
}

void	manage_history(t_shell *state)
{
	char	*hist_entry;
	char	*enc_hist_entry;
	size_t	entry_len;

	if (!state->hist.hist_active)
	{
		buff_readline_reset(&state->readline_buff);
		return ;
	}
	if (state->readline_buff.cursor < 2 || !state->readline_buff.buff.ctx)
	{
		buff_readline_reset(&state->readline_buff);
		return ;
	}
	entry_len = state->readline_buff.cursor - 1;
	/* Skip whitespace-only entries */
	if (is_whitespace_only((char *)state->readline_buff.buff.ctx, entry_len))
	{
		buff_readline_reset(&state->readline_buff);
		return ;
	}
	/* Skip duplicate entries (same as last command) */
	if (is_duplicate_entry(state, (char *)state->readline_buff.buff.ctx,
			entry_len))
	{
		buff_readline_reset(&state->readline_buff);
		return ;
	}
	/* Null-terminate for strdup */
	((char *)state->readline_buff.buff.ctx)[entry_len] = '\0';
	hist_entry = ft_strndup((char *)state->readline_buff.buff.ctx, entry_len);
	if (!hist_entry)
	{
		buff_readline_reset(&state->readline_buff);
		return ;
	}
	add_history(hist_entry);
	vec_push(&state->hist.hist_cmds, &hist_entry);
	/* Write to history file if fd is valid */
	if (state->hist.append_fd >= 0)
	{
		enc_hist_entry = (char *)encode_cmd_hist(hist_entry).ctx;
		if (enc_hist_entry)
		{
			if (write_to_file(enc_hist_entry, state->hist.append_fd))
			{
				warning_error("Failed to write to the history file");
				close(state->hist.append_fd);
				state->hist.append_fd = -1;
			}
			free(enc_hist_entry);
		}
	}
	buff_readline_reset(&state->readline_buff);
}

bool	worthy_of_being_remembered(t_shell *state)
{
	size_t	entry_len;

	if (!state->hist.hist_active)
		return (false);
	if (state->readline_buff.cursor < 2)
		return (false);
	if (!state->readline_buff.buff.ctx)
		return (false);
	entry_len = state->readline_buff.cursor - 1;
	if (is_whitespace_only((char *)state->readline_buff.buff.ctx, entry_len))
		return (false);
	if (is_duplicate_entry(state, (char *)state->readline_buff.buff.ctx,
			entry_len))
		return (false);
	return (true);
}

void	init_history(t_shell *state)
{
	state->hist = (t_history){.append_fd = -1, .hist_active = true};
	vec_init(&state->hist.hist_cmds);
	state->hist.hist_cmds.elem_size = sizeof(char *);
	/* Clear any existing readline history before loading from file */
	rl_clear_history();
	parse_history_file(state);
	/* Setup readline - use default navigation */
	setup_history_search();
}

void	free_hist(t_shell *state)
{
	size_t	i;

	/* Clear readline internal history first to prevent leaks */
	rl_clear_history();

	if (!state->hist.hist_cmds.ctx)
		return ;
	i = 0;
	while (i < state->hist.hist_cmds.len)
	{
		free(((char **)state->hist.hist_cmds.ctx)[i]);
		i++;
	}
	free(state->hist.hist_cmds.ctx);
	state->hist.hist_cmds.ctx = NULL;
	state->hist.hist_cmds.len = 0;
	if (state->hist.append_fd >= 0)
	{
		close(state->hist.append_fd);
		state->hist.append_fd = -1;
	}
}

char	*get_hist_file_path(t_shell *state)
{
	t_env		*env;
	t_string	full_path;

	env = env_get(&state->env, "HOME");
	if (!env || !env->value)
	{
		warning_error("HOME is not set, can't get the history");
		return (NULL);
	}
	vec_init(&full_path);
	full_path.elem_size = 1;
	vec_push_str(&full_path, env->value);
	if (!vec_str_ends_with_str(&full_path, "/"))
	{
		char tmp = '/';
		vec_push(&full_path, &tmp);
	}
	vec_push_str(&full_path, HIST_FILE);
	/* Null-terminate */
	if (!vec_ensure_space_n(&full_path, 1))
	{
		free(full_path.ctx);
		return (NULL);
	}
	((char *)full_path.ctx)[full_path.len] = '\0';
	return ((char *)full_path.ctx);
}

