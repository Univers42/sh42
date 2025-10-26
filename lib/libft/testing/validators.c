/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/22 17:46:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static int is_encoded_value(const void *p)
{
	intptr_t v = (intptr_t)p;
	const intptr_t THRESH = (intptr_t)0xFFFF;
	return (v >= -THRESH && v <= THRESH);
}

/* decode an int result: if value is encoded return it, otherwise read pointed int */
static int decode_int_result(void *result)
{
	if (!result)
		return 0;
	if (is_encoded_value(result))
		return (int)(intptr_t)result;
	return *(int *)result;
}

bool validate_non_null(void *result)
{
	/* encoded 0 is considered null, any non-zero encoded value is non-null */
	if (is_encoded_value(result))
		return ((intptr_t)result != 0);
	return (result != NULL);
}

bool validate_null(void *result)
{
	if (is_encoded_value(result))
		return ((intptr_t)result == 0);
	return (result == NULL);
}

bool validate_positive(void *result)
{
	int v = decode_int_result(result);
	return (v > 0);
}

bool validate_negative(void *result)
{
	int v = decode_int_result(result);
	return (v < 0);
}

bool validate_zero(void *result)
{
	int v = decode_int_result(result);
	return (v == 0);
}

bool validate_even(void *result)
{
	int v = decode_int_result(result);
	return ((v % 2) == 0);
}

bool validate_odd(void *result)
{
	int v = decode_int_result(result);
	return ((v % 2) != 0);
}
