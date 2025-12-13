/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:29:31 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:29:34 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include "ft_stdio.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

char	*getpid_hack(void)
{
	int			fd;
	t_dyn_str	file;
	char		*ret;
	char		**temp;
	const char	*err;

	err = "Cannot get PID.";
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
	{
		warning_error((char *)err);
		return (0);
	}
	dyn_str_init(&file);
	dyn_str_append_fd(fd, &file);
	close(fd);
	temp = ft_split(file.buff, ' ');
	free(file.buff);
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}

t_status	res_status(int status)
{
	t_status	res;

	res.status = status;
	res.pid = -1;
	res.c_c = false;
	return (res);
}

void	init_cwd(t_dyn_str *_cwd)
{
	char	*cwd;

	dyn_str_init(_cwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = getenv("PWD");
	if (cwd)
		dyn_str_pushstr(_cwd, cwd);
	else
		ft_eprintf("Shell-init: getcwd failed: %s\n", strerror(errno));
	free(cwd);
}
