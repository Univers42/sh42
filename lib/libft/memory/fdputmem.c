/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdputmem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:57:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 15:57:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

void	ft_fdputmem(int fd, char *s, int n)
{
	int	i;
	int	written;

	written = 0;
	while (written < n)
	{
		i = write(fd, s + written, n - written);
		if (i < 0)
			break ;
		written += i;
	}
}

void	ft_fdputstr(int fd, char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	ft_fdputmem(fd, s, i);
}

void	ft_putmem(char *s, int n)
{
	ft_fdputmem(1, s, n);
}

void	ft_eputmem(char *s, int n)
{
	ft_fdputmem(2, s, n);
}

bool	str_slice_eq_str(char *s, size_t len, char *s2)
{
	size_t	len_s2;

	len_s2 = ft_strlen(s2);
	if (len != len_s2)
		return (false);
	if (ft_strncmp(s, s2, len) != 0)
		return (true);
	return (false);
}
