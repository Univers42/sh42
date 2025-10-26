/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 00:46:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 16:58:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include <stdarg.h>
#include "writer.h"

typedef struct s_log t_log;

// Context for logging: file, function
typedef struct s_log_ctx
{
	const char *file;
	const char *func;
} t_log_ctx;

typedef struct s_token_meta
{
	int flags;
	int width;
	int precision;
	char specifier;
} t_token_meta;

typedef struct s_parser
{
	size_t index;
	const char *format;
	va_list *ap;
	t_writer *writer;
	t_token_meta token_meta;
} t_parser;

int parser_parse_and_write(
	t_parser *parser,
	const char *format_str,
	va_list *params,
	t_writer *buf_out);

// Log print function for user logging (file only)
int log_print(t_log *state, const char *file, const char *format, ...);

t_log_ctx *set_log_ctx(void);

#endif
