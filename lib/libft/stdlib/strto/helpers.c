/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:03:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 16:06:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conv.h"
#include "ft_ctype.h"

const t_type_info	*get_type_info(t_type type)
{
	size_t					count;
	size_t					i;
	const type_info_entry	*table = get_type_info_table(&count);

	i = -1;
	while (++i < count)
		if (table[i].type == type)
			return ((const t_type_info *)&table[i]);
	return (NULL);
}

// Convert digit character to value
int	char_to_digit(char c, int base)
{
	int	digit;

	if (ft_isdigit((unsigned char)c))
		digit = c - '0';
	else if (ft_isalpha((unsigned char)c))
		digit = (ft_tolower((unsigned char)c) - 'a' + 10);
	else
		return (-1);
	if (digit >= base)
		return (-1);
	return (digit);
}

// Initialize conversion context
void	init_conv_ctx(t_conv_ctx *ctx, const char *nptr, char **endptr,
					int base, t_type type)
{
	ctx->ptr = nptr;
	ctx->start = nptr;
	ctx->endptr = endptr;
	ctx->base = base;
	ctx->type = type;
	ctx->type_info = *get_type_info(type);
	ctx->uval = 0;
	ctx->negative = false;
	ctx->any_digit = false;
	ctx->state = ST_INIT;
}

// Lookup state handler function
t_fn_state	lookup_state_fn(t_state state)
{
	if (state == ST_WHITESPACE)
		return (state_whitespace);
	else if (state == ST_SIGN)
		return (state_sign);
	else if (state == ST_BASE_PREFIX)
		return (state_base_prefix);
	else if (state == ST_DIGITS)
		return (state_digits);
	else if (state == ST_OVERFLOW)
		return (state_overflow);
	else
		return (NULL);
}
