/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:24:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/21 20:23:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>
#include "version_print.h"

int	ft_fprintf(int fd, const char *format, ...);

// Helper to get log state string
static const char	*log_state_str(t_state state)
{
	if (state == ST_WARNING_BASE)
		return ("WARNING");
	else if (state == ST_INFO_BASE)
		return ("INFO");
	else if (state == ST_OK)
		return ("SUCCESS");
	else if (state == ST_ERR_BASE)
		return ("FAILURE");
	else
		return ("LOG");
}

/**
 * In pending
 */
int	ft_vlogprintf(t_log *state, const char *format, va_list *ap)
{
	static t_writer	buf_out;
	int				return_value;
	t_parser		parser;

	if (format == NULL || !state || state->fd < 0)
		return (-1);
	writer_init_fd(&buf_out, state->fd);
	return_value = parser_parse_and_write(&parser, format, ap, &buf_out);
	return (return_value);
}

// Enhanced log_print: accepts file for context
int	log_print(t_log *state, const char *file, const char *format, ...)
{
	va_list	ap;
	int		ret;

	ret = 0;
	if (!state || !file || !format || state->fd < 0)
		return (-1);
	if (state->state == ST_WARNING_BASE || state->state == ST_ERR_BASE)
		ret += ft_fprintf(state->fd, "[%s] %s -> ",
				log_state_str(state->state), file);
	else if (state->state == ST_INFO_BASE || state->state == ST_OK)
		ret += ft_fprintf(state->fd, "[%s] ", log_state_str(state->state));
	else
		ret += ft_fprintf(state->fd, "[LOG] ");
	va_start(ap, format);
	ret += ft_vfprintf(state->fd, format, &ap);
	va_end(ap);
	ret += ft_fprintf(state->fd, "\n");
	return (ret);
}
