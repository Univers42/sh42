/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 18:52:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

void	parse_numeric_escape(char **str)
{
	int	base;
	int	len;
	unsigned char c;

	base = 10;
	len = 0;
	if (**str == '0')
	{
		base = 8;
		len = 3;
	}
	else if (**str == 'x')
	{
		base = 16;
		len = 2;
	}
	else
		return ;
	(*str)++;
	c = ft_strto_long(str, base, len);
	ft_putchar_fd(c, STDOUT_FILENO);
}

static int	backslash_writer(char *s)
{
	if (*s == 'n')
		ft_putchar_fd("\n", STDOUT_FILENO);
	else if (*s == 't')
		ft_putchar_fd("\t", STDOUT_FILENO);
	else if (*s == 'a')
		ft_putchar_fd("\a", STDOUT_FILENO);
	else if (*s == 'b')
		ft_putchar_fd("\b", STDOUT_FILENO);
	else if (*s == 'f')
		ft_putchar_fd("\f", STDOUT_FILENO);
	else if (*s == 'r')
		ft_putchar_fd("\r", STDOUT_FILENO);
	else if (*s == 'v')
		ft_putchar_fd("\v", STDOUT_FILENO);
	else if (*s == '\\')
		ft_putchar_fd("\\", STDOUT_FILENO);
	else if (*s == 'e')
		ft_putchar_fd("\033", STDOUT_FILENO);
	else
		return (0);
	return (1);
}

int	e_parser(char *s)
{
	while (*s)
	{
		if (*s == '\\' && s[1])
		{
			s++;
			if (*s == 'c')
				return (1);
			else if (*s == '0' || *s == 'x')
			{
				parse_numeric_escape(&s);
				continue ;
			}
			else if (!backslash_writer(s))
			{
				ft_putchar_fd("\\", STDOUT_FILENO);
				ft_purchar_fd(*s, STDOUT_FILENO);
			}
			s++;
		}
		else
			ft_putchar_fd(*s++, STDOUT_FILENO);
	}
	return (0);
}


int builtin_echo(t_state *state, t_vec argv)
{
	int		n;
	int		e;
	size_t	first_arg_print;

	n = 0;
	e = 0;
	(void)state;
	first_arg_print = parse_flags(argv, &n, &e);
	if (!print_args(e, argv, first_arg_print) && !n)
		ft_printf("\n");
	return (0);
}