/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:34:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 18:38:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "writer.h"
#include "ft_memory.h"

static int	writer_flush_buf(t_writer *w)
{
	size_t	space;
	size_t	can_copy;

	can_copy = w->index;
	if (w->n_written < (int)w->dst_cap)
		space = w->dst_cap - (size_t)w->n_written;
	else
		space = 0;
	if (can_copy > space)
	{
		can_copy = space;
		w->error = -1;
	}
	if (can_copy > 0 && w->dst != NULL)
		ft_memcpy(w->dst + w->n_written, w->buf, can_copy);
	w->n_written += (int)can_copy;
	w->index = 0;
	return ((int)can_copy);
}

static int	writer_flush_fd(t_writer *w)
{
	size_t	n_written;
	size_t	total_written;

	total_written = 0;
	while (total_written < w->index)
	{
		n_written = write(w->fd, w->buf + total_written,
				w->index - total_written);
		if ((ssize_t)n_written < 0)
			return (w->error = (int)n_written, (int)n_written);
		total_written += n_written;
	}
	w->index = 0;
	w->n_written += (int)total_written;
	return ((int)total_written);
}

int	writer_flush(t_writer *w)
{
	if (w->mode == WRITER_MODE_BUF)
		return (writer_flush_buf(w));
	else
		return (writer_flush_fd(w));
}
