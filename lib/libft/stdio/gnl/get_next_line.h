/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 02:57:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/21 20:11:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include "ft_stdlib.h"
# include "ft_stdio.h"
# include "ft_string.h"
# include "ft_stddef.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define FD_MAX 10000
# define DFLT_CAP 64


typedef struct s_file
{
	char	buf[BUFFER_SIZE];
	char	*cur;
	char	*end;
}	t_file;

typedef struct s_dynstr
{
	char	*buf;
	size_t	size;
	size_t	cap;
}	t_dynstr;

char	*get_next_line(int fd);
char	*get_next_line_bonus(int fd);
t_state	append_from_buffer(t_file *scan, t_dynstr *line);
t_state	refill(t_file *scan, int fd);
t_state	scan_nl(t_file *scan, t_dynstr *line, int fd);

static inline void init(t_file *scan)
{
	if (!scan)
	{
		scan = malloc(sizeof(t_file));
		if (!scan)
			return ;
		scan->cur = scan->buf;
		scan->end = scan->buf;
	}
}

static inline char	*reset(t_dynstr *line, t_file *scan)
{
	if (line)
	{
		free(line->buf);
		line->buf = NULL;
		line->size = 0;
		line->cap = 0;
	}
	if (scan)
	{
		scan->cur = scan->buf;
		scan->end = scan->buf;
	}
	return (NULL);
}

static inline t_state	ensure_cap(char **line, size_t *cap, size_t need)
{
	void	*tmp;
	size_t	new_cap;

	if (*cap >= need)
		return (ST_OK);
	if (*cap)
		new_cap = *cap;
	else
		new_cap = DFLT_CAP;
	while (new_cap <= need)
		new_cap *= 2;
	tmp = ft_realloc(*line, *cap, new_cap);
	if (!tmp)
		return (ST_ERR_ALLOC);
	*cap = new_cap;
	*line = (char *)tmp;
	return (ST_OK);
}

#endif
