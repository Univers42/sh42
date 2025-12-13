/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:31 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:33 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int	write_to_file(char *str, int fd)
{
	int	wrote_total;
	int	wrote;
	int	len;

	len = ft_strlen(str);
	wrote_total = 0;
	while (wrote_total != len)
	{
		wrote = write(fd, str + wrote_total, len - wrote_total);
		if (wrote < 0)
			return (1);
		wrote_total += wrote;
	}
	return (0);
}

static void	default_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	forward_exit_status(t_status res)
{
	ft_assert(res.status != -1);
	if (res.c_c)
	{
		default_signal_handlers();
		kill(0, SIGINT);
		while (true)
			;
	}
	exit(res.status);
}

void	set_cmd_status(t_status *last_cmd_status_res, t_status res,
		char **last_cmd_status_s)
{
	*last_cmd_status_res = res;
	if (*last_cmd_status_s)
		free(*last_cmd_status_s);
	*last_cmd_status_s = ft_itoa(res.status);
}
