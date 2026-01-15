/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/15 15:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arith.h"
#include "shell.h"
#include "env.h"
#include "libft.h"
#include <stdlib.h>

static long	get_var(t_arith_ctx *ctx, const char *name)
{
	char	*val;

	val = env_expand(ctx->state, (char *)name);
	if (!val || !*val)
		return (0);
	return (ft_atoi(val));
}

static void	set_var(t_arith_ctx *ctx, const char *name, long value)
{
	char	*str;

	str = ft_itoa((int)value);
	env_set(&ctx->state->env, (t_env){
		.exported = false,
		.key = ft_strdup(name),
		.value = str
	});
}

static long	parse_ternary(t_arith_ctx *ctx);

static long	parse_primary(t_arith_ctx *ctx)
{
	t_arith_token	*tok;
	long			val;

	tok = &ctx->lexer->current;
	if (tok->type == ARITH_NUM)
	{
		val = tok->value;
		arith_lexer_next(ctx->lexer);
		return (val);
	}
	if (tok->type == ARITH_LPAREN)
	{
		arith_lexer_next(ctx->lexer);
		val = parse_ternary(ctx);
		/* Handle comma inside parentheses */
		while (ctx->lexer->current.type == ARITH_COMMA)
		{
			arith_lexer_next(ctx->lexer);
			val = parse_ternary(ctx);
		}
		if (ctx->lexer->current.type == ARITH_RPAREN)
			arith_lexer_next(ctx->lexer);
		return (val);
	}
	if (tok->type == ARITH_VAR)
	{
		char *name = ft_strdup(tok->name);
		arith_lexer_next(ctx->lexer);
		/* Check for assignment */
		if (ctx->lexer->current.type == ARITH_ASSIGN)
		{
			arith_lexer_next(ctx->lexer);
			val = parse_ternary(ctx);
			set_var(ctx, name, val);
			free(name);
			return (val);
		}
		val = get_var(ctx, name);
		free(name);
		return (val);
	}
	ctx->error = 1;
	return (0);
}

static long	parse_unary(t_arith_ctx *ctx)
{
	t_arith_token	*tok;
	long			val;
	char			*name;

	tok = &ctx->lexer->current;
	if (tok->type == ARITH_PLUS)
		return (arith_lexer_next(ctx->lexer), parse_unary(ctx));
	if (tok->type == ARITH_MINUS)
		return (arith_lexer_next(ctx->lexer), -parse_unary(ctx));
	if (tok->type == ARITH_NOT)
		return (arith_lexer_next(ctx->lexer), !parse_unary(ctx));
	if (tok->type == ARITH_BNOT)
		return (arith_lexer_next(ctx->lexer), ~parse_unary(ctx));
	if (tok->type == ARITH_INC)
	{
		arith_lexer_next(ctx->lexer);
		if (ctx->lexer->current.type == ARITH_VAR)
		{
			name = ft_strdup(ctx->lexer->current.name);
			val = get_var(ctx, name) + 1;
			set_var(ctx, name, val);
			arith_lexer_next(ctx->lexer);
			free(name);
			return (val);
		}
		ctx->error = 1;
		return (0);
	}
	if (tok->type == ARITH_DEC)
	{
		arith_lexer_next(ctx->lexer);
		if (ctx->lexer->current.type == ARITH_VAR)
		{
			name = ft_strdup(ctx->lexer->current.name);
			val = get_var(ctx, name) - 1;
			set_var(ctx, name, val);
			arith_lexer_next(ctx->lexer);
			free(name);
			return (val);
		}
		ctx->error = 1;
		return (0);
	}
	return (parse_primary(ctx));
}

static long	parse_postfix(t_arith_ctx *ctx)
{
	long	val;
	long	old_val;
	char	*name;

	if (ctx->lexer->current.type == ARITH_VAR)
	{
		name = ft_strdup(ctx->lexer->current.name);
		arith_lexer_next(ctx->lexer);
		if (ctx->lexer->current.type == ARITH_INC)
		{
			old_val = get_var(ctx, name);
			set_var(ctx, name, old_val + 1);
			arith_lexer_next(ctx->lexer);
			free(name);
			return (old_val);
		}
		if (ctx->lexer->current.type == ARITH_DEC)
		{
			old_val = get_var(ctx, name);
			set_var(ctx, name, old_val - 1);
			arith_lexer_next(ctx->lexer);
			free(name);
			return (old_val);
		}
		if (ctx->lexer->current.type == ARITH_ASSIGN)
		{
			arith_lexer_next(ctx->lexer);
			val = parse_ternary(ctx);
			set_var(ctx, name, val);
			free(name);
			return (val);
		}
		val = get_var(ctx, name);
		free(name);
		return (val);
	}
	return (parse_unary(ctx));
}

static long	ft_pow(long base, long exp)
{
	long	result;

	result = 1;
	while (exp > 0)
	{
		result *= base;
		exp--;
	}
	return (result);
}

static long	parse_power(t_arith_ctx *ctx)
{
	long	left;

	left = parse_postfix(ctx);
	if (ctx->lexer->current.type == ARITH_POW)
	{
		arith_lexer_next(ctx->lexer);
		return (ft_pow(left, parse_power(ctx)));
	}
	return (left);
}

static long	parse_mul(t_arith_ctx *ctx)
{
	long		left;
	long		right;
	t_arith_tok	op;

	left = parse_power(ctx);
	while (ctx->lexer->current.type == ARITH_MUL ||
		ctx->lexer->current.type == ARITH_DIV ||
		ctx->lexer->current.type == ARITH_MOD)
	{
		op = ctx->lexer->current.type;
		arith_lexer_next(ctx->lexer);
		right = parse_power(ctx);
		if (op == ARITH_MUL)
			left *= right;
		else if (right == 0)
		{
			ft_eprintf("%s: division by 0\n", ctx->state->context);
			ctx->error = 1;
			return (0);
		}
		else if (op == ARITH_DIV)
			left /= right;
		else
			left %= right;
	}
	return (left);
}

static long	parse_add(t_arith_ctx *ctx)
{
	long		left;
	t_arith_tok	op;

	left = parse_mul(ctx);
	while (ctx->lexer->current.type == ARITH_PLUS ||
		ctx->lexer->current.type == ARITH_MINUS)
	{
		op = ctx->lexer->current.type;
		arith_lexer_next(ctx->lexer);
		if (op == ARITH_PLUS)
			left += parse_mul(ctx);
		else
			left -= parse_mul(ctx);
	}
	return (left);
}

static long	parse_shift(t_arith_ctx *ctx)
{
	long		left;
	t_arith_tok	op;

	left = parse_add(ctx);
	while (ctx->lexer->current.type == ARITH_SHL ||
		ctx->lexer->current.type == ARITH_SHR)
	{
		op = ctx->lexer->current.type;
		arith_lexer_next(ctx->lexer);
		if (op == ARITH_SHL)
			left <<= parse_add(ctx);
		else
			left >>= parse_add(ctx);
	}
	return (left);
}

static long	parse_relational(t_arith_ctx *ctx)
{
	long		left;
	t_arith_tok	op;

	left = parse_shift(ctx);
	while (ctx->lexer->current.type == ARITH_LT ||
		ctx->lexer->current.type == ARITH_GT ||
		ctx->lexer->current.type == ARITH_LE ||
		ctx->lexer->current.type == ARITH_GE)
	{
		op = ctx->lexer->current.type;
		arith_lexer_next(ctx->lexer);
		if (op == ARITH_LT)
			left = left < parse_shift(ctx);
		else if (op == ARITH_GT)
			left = left > parse_shift(ctx);
		else if (op == ARITH_LE)
			left = left <= parse_shift(ctx);
		else
			left = left >= parse_shift(ctx);
	}
	return (left);
}

static long	parse_equality(t_arith_ctx *ctx)
{
	long		left;
	t_arith_tok	op;

	left = parse_relational(ctx);
	while (ctx->lexer->current.type == ARITH_EQ ||
		ctx->lexer->current.type == ARITH_NE)
	{
		op = ctx->lexer->current.type;
		arith_lexer_next(ctx->lexer);
		if (op == ARITH_EQ)
			left = left == parse_relational(ctx);
		else
			left = left != parse_relational(ctx);
	}
	return (left);
}

static long	parse_band(t_arith_ctx *ctx)
{
	long	left;

	left = parse_equality(ctx);
	while (ctx->lexer->current.type == ARITH_BAND)
	{
		arith_lexer_next(ctx->lexer);
		left &= parse_equality(ctx);
	}
	return (left);
}

static long	parse_bxor(t_arith_ctx *ctx)
{
	long	left;

	left = parse_band(ctx);
	while (ctx->lexer->current.type == ARITH_BXOR)
	{
		arith_lexer_next(ctx->lexer);
		left ^= parse_band(ctx);
	}
	return (left);
}

static long	parse_bor(t_arith_ctx *ctx)
{
	long	left;

	left = parse_bxor(ctx);
	while (ctx->lexer->current.type == ARITH_BOR)
	{
		arith_lexer_next(ctx->lexer);
		left |= parse_bxor(ctx);
	}
	return (left);
}

static long	parse_and(t_arith_ctx *ctx)
{
	long	left;

	left = parse_bor(ctx);
	while (ctx->lexer->current.type == ARITH_AND)
	{
		arith_lexer_next(ctx->lexer);
		left = left && parse_bor(ctx);
	}
	return (left);
}

static long	parse_or(t_arith_ctx *ctx)
{
	long	left;

	left = parse_and(ctx);
	while (ctx->lexer->current.type == ARITH_OR)
	{
		arith_lexer_next(ctx->lexer);
		left = left || parse_and(ctx);
	}
	return (left);
}

static long	parse_ternary(t_arith_ctx *ctx)
{
	long	cond;
	long	then_val;
	long	else_val;

	cond = parse_or(ctx);
	if (ctx->lexer->current.type == ARITH_QUESTION)
	{
		arith_lexer_next(ctx->lexer);
		then_val = parse_ternary(ctx);
		if (ctx->lexer->current.type == ARITH_COLON)
			arith_lexer_next(ctx->lexer);
		else
			ctx->error = 1;
		else_val = parse_ternary(ctx);
		return (cond ? then_val : else_val);
	}
	return (cond);
}

static long	parse_comma(t_arith_ctx *ctx)
{
	long	left;

	left = parse_ternary(ctx);
	while (ctx->lexer->current.type == ARITH_COMMA)
	{
		arith_lexer_next(ctx->lexer);
		left = parse_ternary(ctx);
	}
	return (left);
}

long	arith_eval_expr(t_arith_ctx *ctx)
{
	return (parse_comma(ctx));
}
