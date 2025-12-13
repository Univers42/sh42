/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:29:22 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 15:13:59 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"

int	get_cols(void)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (80);
	return ((int)ws.ws_col);
}

int	vis_width(const char *s)
{
	mbstate_t	st;
	int			width;

	if (!s)
		return (0);
	ensure_locale();
	ft_memset(&st, 0, sizeof(st));
	width = 0;
	while (*s)
		vis_width_loop(&s, &width, &st);
	return (width);
}

char	*shorten_path(const char *path, int max_w)
{
	t_dyn_str	r;
	const char	*start;
	const char	*last;
	const char	*base;

	if (!path)
		return (ft_strdup("~"));
	dyn_str_init(&r);
	start = tilde_expand(path, &r);
	last = ft_strrchr(start, '/');
	if (last)
		base = last + 1;
	else
		base = start;
	if (vis_width(r.buff) + (int)ft_strlen(start) <= max_w)
		dyn_str_pushstr(&r, start);
	else if (vis_width(base) < max_w - 6 && *base)
	{
		dyn_str_pushstr(&r, "/...");
		if (last)
			dyn_str_pushstr(&r, last);
	}
	else
		dyn_str_pushstr(&r, start);
	return (r.buff);
}

int	cap_cmd(const char *cmd, t_dyn_str *out)
{
	int		pp[2];
	pid_t	pid;
	int		st;

	dyn_str_init(out);
	if (pipe(pp) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(pp[0]);
		close(pp[1]);
		return (-1);
	}
	if (pid == 0)
		child_process_cap(pp, cmd);
	close(pp[1]);
	read_from_pipe(pp[0], out);
	waitpid(pid, &st, 0);
	if (out->len > 0 && out->buff[out->len - 1] == '\n')
		out->buff[--out->len] = '\0';
	if (WIFEXITED(st))
		return (WEXITSTATUS(st));
	return (-1);
}

t_vcs_info	get_git(void)
{
	t_vcs_info	g;
	t_dyn_str	br;
	t_dyn_str	d;
	int			ret;

	g = (t_vcs_info){NULL, 0, 0};
	ret = cap_cmd("git rev-parse --abbrev-ref HEAD 2>/dev/null", &br);
	if (ret == 0 && br.len > 0)
	{
		g.ok = 1;
		g.data = br.buff;
		cap_cmd("git status --porcelain 2>/dev/null | head -1", &d);
		g.dirty = (d.len > 0);
		free(d.buff);
	}
	else
		free(br.buff);
	return (g);
}
