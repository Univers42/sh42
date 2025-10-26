/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:18:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 18:32:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "version_print.h"
#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

/* snprintf-style: cap counts the full destination size including the NUL.
 * Returns number of chars written (excluding NUL). On truncation, returns
 * the number of chars actually placed; sets writer error internally.
 * Reserve space for hte trailing NULL by limiting data capacity
 * Finalize with NULL (best-effort)
*/
int	ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap)
{
	t_writer	buf_out;
	t_parser	parser;
	int			ret;

	if (!dst && cap != 0)
		return (-1);
	if (cap > 0)
		writer_init_buf(&buf_out, dst, cap - 1);
	else
		writer_init_buf(&buf_out, dst, 0);
	ret = parser_parse_and_write(&parser, format, ap, &buf_out);
	writer_terminate_cstr(&buf_out);
	if (ret < 0)
		return (ret);
	return (buf_out.n_written);
}

int	ft_snprintf(char *dst, size_t cap, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, cap, format, &ap);
	va_end(ap);
	return (return_value);
}
