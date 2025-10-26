/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:47:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 14:59:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITER_H
# define WRITER_H

# include <stddef.h>
# include <stdbool.h>

# define BUF_SIZE 2048

typedef enum e_writer_mode
{
	WRITER_MODE_FD = 0,
	WRITER_MODE_BUF = 1
}	t_writer_mode;

typedef struct s_writer
{
	size_t			index;	
	unsigned char	buf[BUF_SIZE];
	int				error;
	int				n_written;
	int				fd;

	/* buffer mode fields */
	t_writer_mode	mode;
	char			*dst;
	size_t			dst_cap; /* bytes available for data (excludes NUL) */
}	t_writer;

bool	writer_write(t_writer *w, const char *mem, size_t size);
bool	writer_write_str(t_writer *w, const char *str);
int		writer_flush(t_writer *w);
bool	writer_is_full(t_writer *w);
bool	writer_putchar(t_writer *w, char c);
void	writer_reset(t_writer *w);
bool	writer_pad_with(t_writer *w, char c, size_t n);
void	writer_write_hex(t_writer *w, size_t n, size_t len, bool upper);
void	writer_write_uint(t_writer *w, unsigned int n, size_t len);

/* new: initialization helpers and C-string termination for buffer mode */
static inline void	writer_init_fd(t_writer *w, int fd)
{
	w->mode = WRITER_MODE_FD;
	w->fd = fd;
	w->dst = NULL;
	w->dst_cap = 0;
	writer_reset(w);
}

static inline void	writer_init_buf(t_writer *w, char *dst, size_t cap_bytes_for_data)
{
	w->mode = WRITER_MODE_BUF;
	w->fd = -1;
	w->dst = dst;
	w->dst_cap = cap_bytes_for_data; /* does NOT include space for NUL */
	writer_reset(w);
}

/* Ensure a trailing '\0' if in buffer mode. Returns 0 on success, -1 on error. */
int		writer_terminate_cstr(t_writer *w);

#endif
