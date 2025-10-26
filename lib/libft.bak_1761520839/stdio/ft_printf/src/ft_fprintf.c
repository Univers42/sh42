/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:15:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/21 20:16:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "version_print.h"
#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

int	ft_vfprintf(int fd, const char *format, va_list *ap)
{
	static t_writer	buf_out;
	int				return_value;
	t_parser		parser;

	if (format == NULL || fd < 0)
		return (-1);
	writer_init_fd(&buf_out, fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vfprintf(fd, format, &ap);
	va_end(ap);
	return (return_value);
}
