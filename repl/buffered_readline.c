/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:27:59 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:02 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"

#include <stdio.h>

#include <sys/wait.h>

#include <unistd.h>

#include "ft_stdio.h"

/* exit codes:
 *
 * 0 - verything ok
 *
 * 1 - ctrl-d
 *
 * 2 - ctrl-c / ctrl-\
 */
void	bg_readline(int outfd, char *prompt)
{
	char	*ret;

	rl_instream = stdin;
	rl_outstream = stderr;
	ret = readline(prompt);
	if (!ret)
	{
		close(outfd);
		exit(1);
	}
	write_to_file(ret, outfd);
	close(outfd);
	exit(0);
}

int	attach_input_readline(t_rl *l, int pp[2], int pid)
{
	int	status;

	close(pp[1]);
	dyn_str_append_fd(pp[0], &l->str);
	buff_readline_update(l);
	close(pp[0]);
	while (1)
	{
		if (waitpid(pid, &status, 0) != -1)
			break ;
	}
	if (WIFSIGNALED(status))
	{
		ft_eprintf("\n");
		return (2);
	}
	return (WEXITSTATUS(status));
}

static void	readline_bg_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

int	get_more_input_readline(t_rl *l, char *prompt)
{
	int	pp[2];
	int	pid;

	if (pipe(pp))
		critical_error_errno_context("pipe");
	pid = fork();
	if (pid == 0)
	{
		readline_bg_signals();
		close(pp[0]);
		bg_readline(pp[1], prompt);
	}
	else if (pid < 0)
		critical_error_errno_context("fork");
	else
	{
		return (attach_input_readline(l, pp, pid));
	}
	ft_assert("Unreachable" == 0);
	return (0);
}
