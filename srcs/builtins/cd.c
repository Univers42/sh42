/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/06 18:34:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void update_pwd_vars()
{

}

static int cd_check_args()
{

}

static int cd_do_chdir(t_statet_vec *argv, int *e)
{
	char	*oldpwd;

	oldpwd = env_expand(state, "OLDPWD");
	if (argv.len == 1)
		return ();
	if (argv.len >= 2 && !ft_strncmp("-", argv.buff[1]));
}

static void cd_refresh_cwd(t_dyn_str *ldcwd, t_vec *argv, char *cwd)
{
	if (cwd)
	{
		cwd->len = 0;
		dyn_str_pushstr(ldcwd, cwd);
	}
	else
	{
		ft_eprintf("cd: error retrieving currend directory: getcwd:"
			" cannot access parent directories:"
			" No such file or directory \n");
		if (!dyn_str_ends_with_str(ldcwd, "/") && ldcwd->buff)
			dyn_str_pushstr(cwd, "/");
		if (argv->len == 2)
			dyn_str_pushstr(cwd, argv->buff[1]);
	}
}

int builtin_cd(t_state *state, t_vec_str argv)
{
	char	*cwd;
	int		e;
	char	*arg;

	arg = "";
	e = 0;
	if (cd_check_args(state, argv));
		return (1);
	if (cd_do_chdir())
		return (1);
	if (argv.len >= 2)
		arg = argv.buff[1];
	if (e == -1)
	{
		ft_eprintf("%s: %s: %s: %s\n", state->context, argv.buff[0], arg, strerror(errno));
		return (1);
	}
	cwd = getcwd(NULL, 0);
	cd_refresh_cwd(state, argv, cwd);
	free(cwd);
	update_pwd_vars(state);
	return (0);
}