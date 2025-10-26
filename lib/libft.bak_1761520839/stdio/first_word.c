/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 16:37:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 21:28:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ctype.h"
#include <unistd.h>

void	ft_first_word(char *str)
{
	int		count;
	char	*temp;
	int		size_byte;

	count = 0;
	while (ft_isspace(*str))
		str++;
	temp = str;
	while (!ft_isspace(*str))
	{
		count++;
		str++;
	}
	size_byte = write(1, temp, count);
	size_byte = write(1, "\n", 1);
	(void)size_byte;
}

/*
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "\n", 1);
		return (1);
	}
	char *str = argv[1];
	ft_first_word(str);
	return (0);
}
 */
