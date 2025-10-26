/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:01:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 16:02:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conv.h"

// Finalize conversion and apply type constraints
static int64_t	finalize_negative_conversion(t_conv_ctx *ctx)
{
	int64_t	result;

	if (ctx->uval > (uint64_t)-(ctx->type_info.min_val))
	{
		errno = ERANGE;
		return (ctx->type_info.min_val);
	}
	result = (-(int64_t)ctx->uval);
	return (result);
}

static int64_t	finalize_positive_conversion(t_conv_ctx *ctx)
{
	int64_t	result;

	if (ctx->uval > ctx->type_info.max_val)
	{
		errno = ERANGE;
		return (ctx->type_info.max_val);
	}
	result = (int64_t)ctx->uval;
	return (result);
}

static int64_t	finalize_conversion(t_conv_ctx *ctx)
{
	int64_t	result;

	if (!ctx->any_digit)
	{
		if (ctx->endptr)
			*ctx->endptr = (char *)ctx->start;
		return (0);
	}
	if (ctx->endptr)
		*ctx->endptr = (char *)ctx->ptr;
	if (ctx->negative && !ctx->type_info.is_signed)
	{
		errno = ERANGE;
		return (0);
	}
	if (ctx->negative)
		result = finalize_negative_conversion(ctx);
	else
		result = finalize_positive_conversion(ctx);
	return (result);
}

const t_type_info	*get_type_info(t_type type)
{
	size_t					count;
	const type_info_entry	*table = get_type_info_table(&count);
	size_t					i;

	i = -1;
	while (++i < count)
		if (table[i].type == type)
			return ((const t_type_info *)&table[i]);
	return (NULL);
}

// UNIFIED CONVERSION FUNCTION - Handles all integer types
int64_t	ft_strto(const char *nptr, char **endptr, int base, t_type type)
{
	t_conv_ctx	ctx;
	t_fn_state	fn;

	if (base < 0 || base == 1 || base > 36)
	{
		errno = EINVAL;
		if (endptr)
			*endptr = (char *)nptr;
		return (0);
	}
	init_conv_ctx(&ctx, nptr, endptr, base, type);
	if (ctx.state == ST_INIT)
		ctx.state = ST_WHITESPACE;
	while (ctx.state != ST_DONE && ctx.state != ST_ERR_BASE)
	{
		fn = lookup_state_fn(ctx.state);
		if (!fn)
			break ;
		fn(&ctx);
	}
	return (finalize_conversion(&ctx));
}
