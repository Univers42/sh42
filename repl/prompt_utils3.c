/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 16:06:00 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 14:12:03 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

const char	*tilde_expand(const char *path, t_dyn_str *r)
{
	const char	*home;

	home = getenv("HOME");
	if (home && ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		dyn_str_pushstr(r, "~");
		return (path + ft_strlen(home));
	}
	return (path);
}

void	child_process_cap(int *pp, const char *cmd)
{
	close(pp[0]);
	dup2(pp[1], STDOUT_FILENO);
	close(pp[1]);
	execl("/bin/sh", "sh", "-c", cmd, NULL);
	exit(127);
}

void	read_from_pipe(int fd, t_dyn_str *out)
{
	char	buff[1024];
	ssize_t	ret;

	while (1)
	{
		ret = read(fd, buff, sizeof(buff) - 1);
		if (ret <= 0)
		{
			break ;
		}
		buff[ret] = '\0';
		dyn_str_pushstr(out, buff);
	}
	close(fd);
}
