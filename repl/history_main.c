/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:39 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:50:35 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include <fcntl.h>
#include <unistd.h>
#include "ft_vector.h"
#include "var.h"

static t_dyn_str	parse_single_cmd(t_dyn_str hist, size_t *cur)
{
	t_dyn_str	cmd;
	bool		bs;
	char		c;

	dyn_str_init(&cmd);
	bs = false;
	while (*cur < hist.len)
	{
		c = hist.buff[(*cur)++];
		if (!bs && c == '\\')
			bs = true;
		else if (!bs && c == '\n')
			break ;
		else
		{
			dyn_str_push(&cmd, c);
			bs = false;
		}
	}
	return (cmd);
}

static t_vec	parse_hist_file(t_dyn_str hist)
{
	size_t	cur;
	t_vec	ret;
	char	*cmd;

	cur = 0;
	vec_init(&ret, &(t_vec_config){
		.elem_size = sizeof(char *),
		.initial_capacity = 64,
		.type_mask = VEC_TYPE_PTR});
	while (cur < hist.len)
	{
		cmd = parse_single_cmd(hist, &cur).buff;
		vec_push(&ret, &cmd);
		add_history(cmd);
	}
	return (ret);
}

char	*get_hist_file_path(t_vec *_env)
{
	t_env		*env;
	t_dyn_str	full_path;

	env = env_get(_env, "HOME");
	if (!env || !env->value)
	{
		warning_error("HOME is not set, can't get the history");
		return (0);
	}
	dyn_str_init(&full_path);
	dyn_str_pushstr(&full_path, env->value);
	if (!dyn_str_ends_with_str(&full_path, "/"))
		dyn_str_push(&full_path, '/');
	dyn_str_pushstr(&full_path, HIST_FILE);
	return (full_path.buff);
}

static void	load_history_from_file(t_hist *hist, const char *path)
{
	t_dyn_str	hist_content;
	int			fd;

	fd = open(path, O_RDONLY | O_CREAT, 0666);
	if (fd < 0)
	{
		warning_error("Can't open the history file for reading");
		return ;
	}
	dyn_str_init(&hist_content);
	dyn_str_append_fd(fd, &hist_content);
	close(fd);
	hist->cmds = parse_hist_file(hist_content);
	free(hist_content.buff);
}

void	parse_history_file(t_hist *hist, t_vec *env)
{
	char	*hist_file_path;

	hist_file_path = get_hist_file_path(env);
	if (!hist_file_path)
		return ;
	load_history_from_file(hist, hist_file_path);
	hist->append_fd = open(hist_file_path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (hist->append_fd < 0)
		warning_error("Can't open the history file for writing");
	free(hist_file_path);
}
