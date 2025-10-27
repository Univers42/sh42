/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:54:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/26 17:56:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token_type check_key_word(int start, int length, const char *rest, t_token_type type, t_scanner *scan)
{
	if (scan->current - scan->start == start + length && ft_memcmp(scan->start + start, rest, length) == 0)
		return (type);
	return (TOKEN_IDENTIFIER);
}

t_token_type identifier_type(t_scanner *scan)
{
	int length = (int)(scan->current - scan->start);
	const t_keyword_entry *kw = map_keywords();

	for (int i = 0; kw[i].keyword != NULL; i++)
	{
		if (kw[i].length == length &&
			ft_memcmp(scan->start, kw[i].keyword, length) == 0)
			return kw[i].type;
	}
	return TOKEN_IDENTIFIER;
}

t_token identifier(t_scanner *scan)
{
	// Only consume [a-zA-Z0-9_], not punctuation
	while (ft_isalpha(peek(scan)) || ft_isdigit(peek(scan)) || peek(scan) == '_')
		advance(scan);
	return (make_token(scan, identifier_type(scan)));
}

t_token number(t_scanner *scan)
{
	while (ft_isdigit(peek(scan)))
		advance(scan);
	if (peek(scan) == '.' && ft_isdigit(peek_next(scan)))
	{
		advance(scan);
		while (ft_isdigit(peek(scan)))
			advance(scan);
	}
	return (make_token(scan, TOKEN_NUMBER));
}

t_token string(t_scanner *scan)
{
	while (peek(scan) != '"' && !scan_is_at_end(scan))
	{
		if (peek(scan) == '\n')
			scan->line++;
		advance(scan);
	}
	if (scan_is_at_end(scan))
		return (error_token(scan, "Unterminated string"));
	advance(scan);
	return (make_token(scan, TOKEN_STRING));
}
