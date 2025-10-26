/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:45:51 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 00:30:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * allocate unused space for an object whose size in bytes is specifies by
 * size  and whose value is unspecified
 * @param n size of allocation
 * @return size == 0 ?
 * 	"pointer to alloc space" : "freeable pointer of any
 * 	source or NULL ptr with errno"
 * @ERRORS:
 * [ENOMEM] insufficient storage space is available
 */
void	*ft_malloc(size_t n)
{
	return (NULL);
}
