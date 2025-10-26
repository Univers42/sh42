/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stub2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:02:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 00:29:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conv.h"

unsigned int	ft_strto_uint(const char *nptr, char **endptr, int base)
{
	return ((unsigned int)ft_strto(nptr, endptr, base, TYPE_UINT));
}

int	ft_strto_int(const char *nptr, char **endptr, int base)
{
	return ((int)ft_strto(nptr, endptr, base, TYPE_INT));
}

unsigned long	ft_strto_ulong(const char *nptr, char **endptr, int base)
{
	return ((unsigned long)ft_strto(nptr, endptr, base, TYPE_ULONG));
}

long	ft_strto_long(const char *nptr, char **endptr, int base)
{
	return ((long)ft_strto(nptr, endptr, base, TYPE_LONG));
}

uint8_t	ft_strto_u8(const char *nptr, char **endptr, int base)
{
	return ((uint8_t)ft_strto(nptr, endptr, base, TYPE_UINT8));
}
