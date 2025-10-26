/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:47:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 18:34:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "writer.h"
#include "ft_memory.h"

/*
 * Does not zero initialize the buffer. Zero initializes the variables.
 */
void	writer_reset(t_writer *w)
{
	w->index = 0;
	w->error = 0;
	w->n_written = 0;
}

bool	writer_write(t_writer *w, const char *mem, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size && writer_putchar(w, mem[i]))
		++i;
	return (w->error >= 0);
}

bool	writer_is_full(t_writer *w)
{
	return (w->index == BUF_SIZE);
}

bool	writer_putchar(t_writer *w, char c)
{
	if (writer_is_full(w) && writer_flush(w) < 0)
		return (false);
	w->buf[w->index] = c;
	w->index++;
	return (true);
}

int	writer_terminate_cstr(t_writer *w)
{
	if (w->mode != WRITER_MODE_BUF)
		return (0);
	if (w->index && writer_flush(w) < 0 && w->dst_cap == 0)
		return (-1);
	if (w->dst && (size_t)w->n_written < w->dst_cap + 1)
	{
		if ((size_t)w->n_written < w->dst_cap)
			w->dst[w->n_written] = '\0';
		else
			w->dst[w->dst_cap] = '\0';
		return (0);
	}
	w->error = -1;
	return (-1);
}
