/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runners.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:55:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/22 20:01:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* forward declaration so run_var_test can detect encoded scalar values */
static int is_encoded_value(const void *p);

void run_fn_batch(t_suite *suite, const char *batch_name,
				  void *fn_test, void *fn_ref, t_case *cases,
					size_t count)
{
	size_t i;
	char test_name[256];

	printf("\n\033[1;34m=== %s ===\033[0m\n", batch_name);
	i = -1;
	while (++i < count)
	{
		if (cases[i].description)
			ft_snprintf(test_name, sizeof(test_name), "%s [%zu]",
						cases[i].description, i);
		else
			ft_snprintf(test_name, sizeof(test_name), "test [%zu]", i);
		run_fn_test(suite, test_name, fn_test, fn_ref, &cases[i]);
	}
}

void run_edge_batch(t_suite *suite, const char *batch_name,
					void *fn_test, t_edge_case *cases, size_t count)
{
	size_t i;
	char test_name[256];

	printf("\n\033[1;36m=== %s (Edge Cases) ===\033[0m\n", batch_name);
	i = 0;
	while (i < count)
	{
		if (cases[i].description)
			snprintf(test_name, sizeof(test_name), "%s [%zu]",
					 cases[i].description, i);
		else
			snprintf(test_name, sizeof(test_name), "edge [%zu]", i);
		run_edge_test(suite, test_name, fn_test, &cases[i]);
		i++;
	}
}

void run_var_test(t_suite *suite, const char *name,
				  const void *actual, const void *expected, t_type type, size_t n)
{
	t_test result;
	int cmp_res;
	size_t size = n;

	memset(&result, 0, sizeof(t_test));
	result.test_name = (char *)name;
	result.type = type;

	/* determine copy size or method */
	if (type == TYPE_STRING)
	{
		/* strdup strings so they survive caller stack */
		result.actual = actual ? strdup((const char *)actual) : NULL;
		result.expected = expected ? strdup((const char *)expected) : NULL;
		result.actual_owned = (result.actual != NULL);
		result.expected_owned = (result.expected != NULL);
	}
	else
	{
		if (size == 0)
		{
			/* fallback: infer size from type for common types */
			if (type == TYPE_INT || type == TYPE_UINT || type == TYPE_BOOL)
				size = sizeof(int);
			else if (type == TYPE_LONG || type == TYPE_ULONG)
				size = sizeof(long);
			else if (type == TYPE_FLOAT)
				size = sizeof(float);
			else if (type == TYPE_DOUBLE)
				size = sizeof(double);
			else
				size = sizeof(void *);
		}
		/* if actual is an encoded scalar, store it directly (no memcpy) */
		if (actual)
		{
			if (is_encoded_value(actual))
			{
				result.actual = (void *)actual;
				result.actual_owned = FALSE;
			}
			else
			{
				result.actual = malloc(size);
				if (result.actual)
					memcpy(result.actual, actual, size);
				result.actual_owned = (result.actual != NULL);
			}
		}
		else
			result.actual = NULL;

		/* if expected is an encoded scalar, store it directly (no memcpy) */
		if (expected)
		{
			if (is_encoded_value(expected))
			{
				result.expected = (void *)expected;
				result.expected_owned = FALSE;
			}
			else
			{
				result.expected = malloc(size);
				if (result.expected)
					memcpy(result.expected, expected, size);
				result.expected_owned = (result.expected != NULL);
			}
		}
		else
			result.expected = NULL;
	}

	result.actual_origin = ORIGIN_LITERAL;
	result.expected_origin = ORIGIN_LITERAL;

	cmp_res = cmp_values(result.actual ? result.actual : actual,
						 result.expected ? result.expected : expected,
						 type, size);
	result.st = (cmp_res == 0) ? TEST_PASS : TEST_FAIL;
	if (cmp_res != 0)
		result.error_msg = strdup("values differ");
	suite_record(suite, result);
}

/* treat small pointer values (encoded integers) as scalar results */
static int is_encoded_value(const void *p)
{
	intptr_t v = (intptr_t)p;
	const intptr_t THRESH = (intptr_t)0xFFFF;
	return (v >= -THRESH && v <= THRESH);
}

/* return truthiness of value pointed by p or encoded in pointer.
   n is size in bytes (0 => default sizeof(int)). */
static bool get_bool_value(const void *p, size_t n)
{
	uint64_t tmp = 0;

	if (!p)
		return (false);
	if (is_encoded_value(p))
		return ((intptr_t)p != 0);
	if (n == 0)
		n = sizeof(int);
	if (n > sizeof(tmp))
		n = sizeof(tmp);
	memcpy(&tmp, p, n);
	return (tmp != 0);
}

static int cmp_int_types(const void *a, const void *b, t_type type)
{
	/* if either side is an encoded scalar, compare by value */
	if (is_encoded_value(a) || is_encoded_value(b))
	{
		long va = (long)(intptr_t)a;
		long vb = (long)(intptr_t)b;
		if (type == TYPE_CHAR)
			return ((char)va - (char)vb);
		if (type == TYPE_UCHAR)
			return ((unsigned char)va - (unsigned char)vb);
		if (type == TYPE_SHORT)
			return ((short)va - (short)vb);
		if (type == TYPE_USHORT)
			return ((unsigned short)va - (unsigned short)vb);
		if (type == TYPE_INT)
			return ((int)va - (int)vb);
		if (type == TYPE_UINT)
			return ((unsigned int)va > (unsigned int)vb) - ((unsigned int)va < (unsigned int)vb);
		return (0);
	}
	/* otherwise treat as pointers to data */
	if (type == TYPE_CHAR)
		return (*(char *)a - *(char *)b);
	if (type == TYPE_UCHAR)
		return (*(unsigned char *)a - *(unsigned char *)b);
	if (type == TYPE_SHORT)
		return (*(short *)a - *(short *)b);
	if (type == TYPE_USHORT)
		return (*(unsigned short *)a - *(unsigned short *)b);
	if (type == TYPE_INT)
		return (*(int *)a - *(int *)b);
	if (type == TYPE_UINT)
		return ((*(unsigned int *)a > *(unsigned int *)b) - (*(unsigned int *)a < *(unsigned int *)b));
	return (0);
}

static int cmp_long_types(const void *a, const void *b, t_type type)
{
	long la;
	long lb;

	if (is_encoded_value(a) || is_encoded_value(b))
	{
		la = (long)(intptr_t)a;
		lb = (long)(intptr_t)b;
		if (type == TYPE_LONG)
			return ((la > lb) - (la < lb));
		if (type == TYPE_ULONG)
			return ((unsigned long)la > (unsigned long)lb) - ((unsigned long)la < (unsigned long)lb);
		return (0);
	}
	if (type == TYPE_LONG)
	{
		la = *(long *)a;
		lb = *(long *)b;
		return ((la > lb) - (la < lb));
	}
	if (type == TYPE_ULONG)
		return ((*(unsigned long *)a > *(unsigned long *)b) - (*(unsigned long *)a < *(unsigned long *)b));
	return (0);
}

static int cmp_float_types(const void *a, const void *b, t_type type)
{
	/* If values are encoded scalars, interpret them as integer-returned values.
	   For float/double returns encoded in pointer (rare), cast integer to float/double. */
	if (is_encoded_value(a) || is_encoded_value(b))
	{
		long va = (long)(intptr_t)a;
		long vb = (long)(intptr_t)b;
		if (type == TYPE_FLOAT)
		{
			float fa = (float)va;
			float fb = (float)vb;
			float diff_f = fa - fb;
			if (fabsf(diff_f) < 1e-6f)
				return (0);
			return ((diff_f > 0) - (diff_f < 0));
		}
		if (type == TYPE_DOUBLE)
		{
			double da = (double)va;
			double db = (double)vb;
			double diff_d = da - db;
			if (fabs(diff_d) < 1e-9)
				return (0);
			return ((diff_d > 0) - (diff_d < 0));
		}
		return (0);
	}
	/* normal pointer-to-data path */
	{
		float diff_f;
		double diff_d;

		if (type == TYPE_FLOAT)
		{
			diff_f = *(float *)a - *(float *)b;
			if (fabsf(diff_f) < 1e-6)
				return (0);
			return ((diff_f > 0) - (diff_f < 0));
		}
		if (type == TYPE_DOUBLE)
		{
			diff_d = *(double *)a - *(double *)b;
			if (fabs(diff_d) < 1e-9)
				return (0);
			return ((diff_d > 0) - (diff_d < 0));
		}
	}
	return (0);
}

int cmp_values(const void *a, const void *b, t_type type, size_t n)
{
	int result;

	if (!a && !b)
		return (0);
	if (!a || !b)
		return (-999);
	if (type == TYPE_PTR)
		return ((a == b) ? 0 : 1);
	if (type == TYPE_STRING)
		return (strcmp((const char *)a, (const char *)b));
	/* handle TYPE_BOOL by testing truthiness of the full integer value */
	if (type == TYPE_BOOL)
	{
		bool ba = get_bool_value(a, n);
		bool bb = get_bool_value(b, n);
		return (ba == bb) ? 0 : 1;
	}
	result = cmp_int_types(a, b, type);
	if (result != 0 || type <= TYPE_UINT)
		return (result);
	result = cmp_long_types(a, b, type);
	if (result != 0 || type <= TYPE_ULONG)
		return (result);
	result = cmp_float_types(a, b, type);
	if (result != 0 || type <= TYPE_DOUBLE)
		return (result);
	return (memcmp(a, b, n));
}
