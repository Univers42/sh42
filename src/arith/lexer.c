/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/15 15:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"
#include "libft.h"
#include <stdlib.h>

static void	skip_whitespace(t_arith_lexer *lex)
{
	while (lex->pos < lex->len && ft_isspace(lex->input[lex->pos]))
		lex->pos++;
}

static int	lex_number(t_arith_lexer *lex)
{
	long	val;
	int		base;
	int		digit;
	char	c;

	val = 0;
	base = 10;
	if (lex->input[lex->pos] == '0' && lex->pos + 1 < lex->len)
	{
		if (lex->input[lex->pos + 1] == 'x' || lex->input[lex->pos + 1] == 'X')
		{
			base = 16;
			lex->pos += 2;
		}
		else if (ft_isdigit(lex->input[lex->pos + 1]))
		{
			base = 8;
			lex->pos++;
		}
	}
	while (lex->pos < lex->len)
	{
		digit = -1;
		c = lex->input[lex->pos];
		if (ft_isdigit(c))
			digit = c - '0';
		else if (base == 16 && c >= 'a' && c <= 'f')
			digit = c - 'a' + 10;
		else if (base == 16 && c >= 'A' && c <= 'F')
			digit = c - 'A' + 10;
		if (digit < 0 || digit >= base)
			break ;
		val = val * base + digit;
		lex->pos++;
	}
	lex->current.type = ARITH_NUM;
	lex->current.value = val;
	return (0);
}

static int	lex_ident(t_arith_lexer *lex)
{
	int	start;

	start = lex->pos;
	while (lex->pos < lex->len &&
		(ft_isalnum(lex->input[lex->pos]) || lex->input[lex->pos] == '_'))
		lex->pos++;
	lex->current.type = ARITH_VAR;
	lex->current.name = ft_strndup(lex->input + start, lex->pos - start);
	return (0);
}

static int	lex_two_char_op(t_arith_lexer *lex, char c, char c2)
{
	if (c == '&' && c2 == '&')
		{ lex->current.type = ARITH_AND; lex->pos += 2; return (1); }
	if (c == '|' && c2 == '|')
		{ lex->current.type = ARITH_OR; lex->pos += 2; return (1); }
	if (c == '=' && c2 == '=')
		{ lex->current.type = ARITH_EQ; lex->pos += 2; return (1); }
	if (c == '!' && c2 == '=')
		{ lex->current.type = ARITH_NE; lex->pos += 2; return (1); }
	if (c == '<' && c2 == '=')
		{ lex->current.type = ARITH_LE; lex->pos += 2; return (1); }
	if (c == '>' && c2 == '=')
		{ lex->current.type = ARITH_GE; lex->pos += 2; return (1); }
	if (c == '<' && c2 == '<')
		{ lex->current.type = ARITH_SHL; lex->pos += 2; return (1); }
	if (c == '>' && c2 == '>')
		{ lex->current.type = ARITH_SHR; lex->pos += 2; return (1); }
	if (c == '+' && c2 == '+')
		{ lex->current.type = ARITH_INC; lex->pos += 2; return (1); }
	if (c == '-' && c2 == '-')
		{ lex->current.type = ARITH_DEC; lex->pos += 2; return (1); }
	if (c == '*' && c2 == '*')
		{ lex->current.type = ARITH_POW; lex->pos += 2; return (1); }
	return (0);
}

static int	lex_one_char_op(t_arith_lexer *lex, char c)
{
	if (c == '+') { lex->current.type = ARITH_PLUS; lex->pos++; return (1); }
	if (c == '-') { lex->current.type = ARITH_MINUS; lex->pos++; return (1); }
	if (c == '*') { lex->current.type = ARITH_MUL; lex->pos++; return (1); }
	if (c == '/') { lex->current.type = ARITH_DIV; lex->pos++; return (1); }
	if (c == '%') { lex->current.type = ARITH_MOD; lex->pos++; return (1); }
	if (c == '<') { lex->current.type = ARITH_LT; lex->pos++; return (1); }
	if (c == '>') { lex->current.type = ARITH_GT; lex->pos++; return (1); }
	if (c == '=') { lex->current.type = ARITH_ASSIGN; lex->pos++; return (1); }
	if (c == '!') { lex->current.type = ARITH_NOT; lex->pos++; return (1); }
	if (c == '~') { lex->current.type = ARITH_BNOT; lex->pos++; return (1); }
	if (c == '&') { lex->current.type = ARITH_BAND; lex->pos++; return (1); }
	if (c == '|') { lex->current.type = ARITH_BOR; lex->pos++; return (1); }
	if (c == '^') { lex->current.type = ARITH_BXOR; lex->pos++; return (1); }
	if (c == '(') { lex->current.type = ARITH_LPAREN; lex->pos++; return (1); }
	if (c == ')') { lex->current.type = ARITH_RPAREN; lex->pos++; return (1); }
	if (c == '?') { lex->current.type = ARITH_QUESTION; lex->pos++; return (1); }
	if (c == ':') { lex->current.type = ARITH_COLON; lex->pos++; return (1); }
	if (c == ',') { lex->current.type = ARITH_COMMA; lex->pos++; return (1); }
	return (0);
}

static int	lex_operator(t_arith_lexer *lex)
{
	char	c;
	char	c2;

	c = lex->input[lex->pos];
	c2 = (lex->pos + 1 < lex->len) ? lex->input[lex->pos + 1] : 0;
	if (lex_two_char_op(lex, c, c2))
		return (0);
	if (lex_one_char_op(lex, c))
		return (0);
	return (-1);
}

void	arith_lexer_init(t_arith_lexer *lex, const char *input, int len)
{
	lex->input = input;
	lex->len = len;
	lex->pos = 0;
	lex->error = 0;
	lex->current.type = ARITH_EOF;
	lex->current.name = NULL;
}

int	arith_lexer_next(t_arith_lexer *lex)
{
	arith_lexer_free_token(&lex->current);
	skip_whitespace(lex);
	if (lex->pos >= lex->len)
	{
		lex->current.type = ARITH_EOF;
		return (0);
	}
	if (ft_isdigit(lex->input[lex->pos]))
		return (lex_number(lex));
	if (ft_isalpha(lex->input[lex->pos]) || lex->input[lex->pos] == '_')
		return (lex_ident(lex));
	return (lex_operator(lex));
}

void	arith_lexer_free_token(t_arith_token *tok)
{
	if (tok->name)
	{
		free(tok->name);
		tok->name = NULL;
	}
}
