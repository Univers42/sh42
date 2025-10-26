/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ranges.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:59:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 16:01:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

ssize_t	ft_abs(ssize_t n)
{
	if (n < 0)
		return (-n);
	return (n);
}

ssize_t	ft_clamp(ssize_t n, ssize_t min, ssize_t max)
{
	if (n < min)
		return (min);
	if (n > max)
		return (max);
	return (n);
}

ssize_t	ft_smin(ssize_t a, ssize_t b)
{
	return ((a > b) * b + (a <= b) * a);
}

size_t	ft_smax(ssize_t a, ssize_t b)
{
	return ((a < b) * b + (a >= b) * a);
}
