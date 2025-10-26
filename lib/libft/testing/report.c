/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:54:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/22 17:47:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

static void print_header(const char *title)
{
	printf("\n\033[1;35m╔══════════════════════════════════════════╗\033[0m\n");
	printf("\033[1;35m║  %-38s  ║\033[0m\n", title);
	printf("\033[1;35m╚══════════════════════════════════════════╝\033[0m\n");
}

static void print_stats(const t_suite *suite)
{
	printf("\nSuite: \033[1;36m%s\033[0m\n", suite->name);
	printf("Total tests: %u\n", suite->total);
	printf("\033[1;32mPassed: %u\033[0m\n", suite->passed);
	printf("\033[1;31mFailed: %u\033[0m\n", suite->failed);
	if (suite->error > 0)
		printf("\033[1;33mErrors: %u\033[0m\n", suite->error);
	if (suite->skipped_tests > 0)
		printf("Skipped: %u\n", suite->skipped_tests);
}

void suite_print_summary(const t_suite *suite)
{
	double pass_rate;

	print_header("TEST SUITE SUMMARY");
	print_stats(suite);
	if (suite->total > 0)
		pass_rate = (100.0 * suite->passed) / suite->total;
	else
		pass_rate = 0.0;
	printf("\nSuccess rate: %.2f%%\n", pass_rate);
}

static int is_encoded_value(const void *p)
{
	intptr_t v = (intptr_t)p;
	const intptr_t THRESH = (intptr_t)0xFFFF;
	return (v >= -THRESH && v <= THRESH);
}

/* decode boolean-like value safely (reads up to n bytes or sizeof(int) if n==0) */
static bool bool_value_from_ptr(const void *p, size_t n)
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

/* return malloc'd string describing value */
static char *format_value(void *v, t_type type)
{
	char buf[256];
	if (!v)
		return strdup("NULL");
	/* handle encoded scalar results */
	if (is_encoded_value(v))
	{
		intptr_t val = (intptr_t)v;
		switch (type)
		{
		case TYPE_CHAR:
			ft_snprintf(buf, sizeof(buf), "%d (encoded char)", (int)(char)val);
			break;
		case TYPE_UCHAR:
			snprintf(buf, sizeof(buf), "%u (encoded uchar)", (unsigned int)(unsigned char)val);
			break;
		case TYPE_SHORT:
			ft_snprintf(buf, sizeof(buf), "%d (encoded short)", (int)(short)val);
			break;
		case TYPE_USHORT:
			snprintf(buf, sizeof(buf), "%u (encoded ushort)", (unsigned int)(unsigned short)val);
			break;
		case TYPE_INT:
			snprintf(buf, sizeof(buf), "%ld (encoded int)", (long)val);
			break;
		case TYPE_UINT:
			snprintf(buf, sizeof(buf), "%lu (encoded uint)", (unsigned long)val);
			break;
		case TYPE_LONG:
			snprintf(buf, sizeof(buf), "%ld (encoded long)", (long)val);
			break;
		case TYPE_ULONG:
			snprintf(buf, sizeof(buf), "%lu (encoded ulong)", (unsigned long)val);
			break;
		case TYPE_FLOAT:
			snprintf(buf, sizeof(buf), "%f (encoded float)", (double)(float)val);
			break;
		case TYPE_DOUBLE:
			snprintf(buf, sizeof(buf), "%f (encoded double)", (double)val);
			break;
		case TYPE_STRING:
			/* encoded pointer but requested string: print numeric value */
			snprintf(buf, sizeof(buf), "%ld (encoded ptr)", (long)val);
			break;
		case TYPE_BOOL:
			snprintf(buf, sizeof(buf), "%s (encoded bool)", (val ? "true" : "false"));
			break;
		default:
			snprintf(buf, sizeof(buf), "%p (encoded)", v);
			break;
		}
		return strdup(buf);
	}
	/* real pointer values */
	switch (type)
	{
	case TYPE_STRING:
		return strdup((const char *)v);
	case TYPE_BOOL:
		/* use truthiness across the integer-sized value instead of dereferencing bool */
		ft_snprintf(buf, sizeof(buf), "%s", bool_value_from_ptr(v, sizeof(int)) ? "true" : "false");
		return strdup(buf);
	case TYPE_CHAR:
		ft_snprintf(buf, sizeof(buf), "%d", (int)*(char *)v);
		return strdup(buf);
	case TYPE_UCHAR:
		snprintf(buf, sizeof(buf), "%u", (unsigned int)*(unsigned char *)v);
		return strdup(buf);
	case TYPE_SHORT:
		ft_snprintf(buf, sizeof(buf), "%d", (int)*(short *)v);
		return strdup(buf);
	case TYPE_USHORT:
		snprintf(buf, sizeof(buf), "%u", (unsigned int)*(unsigned short *)v);
		return strdup(buf);
	case TYPE_INT:
		ft_snprintf(buf, sizeof(buf), "%d", *(int *)v);
		return strdup(buf);
	case TYPE_UINT:
		snprintf(buf, sizeof(buf), "%u", *(unsigned int *)v);
		return strdup(buf);
	case TYPE_LONG:
		snprintf(buf, sizeof(buf), "%ld", *(long *)v);
		return strdup(buf);
	case TYPE_ULONG:
		snprintf(buf, sizeof(buf), "%lu", *(unsigned long *)v);
		return strdup(buf);
	case TYPE_FLOAT:
		snprintf(buf, sizeof(buf), "%f", *(float *)v);
		return strdup(buf);
	case TYPE_DOUBLE:
		snprintf(buf, sizeof(buf), "%f", *(double *)v);
		return strdup(buf);
	case TYPE_PTR:
	default:
		ft_snprintf(buf, sizeof(buf), "%p", v);
		return strdup(buf);
	}
}

static const char *origin_to_str(t_origin o)
{
	if (o == ORIGIN_LITERAL)
		return "literal";
	if (o == ORIGIN_USER_FN)
		return "user fn";
	if (o == ORIGIN_REF_FN)
		return "ref fn";
	return "unknown";
}

void suite_print_detailed(const t_suite *suite)
{
	uint16_t	i;
	t_test		*r;
	char		*exp_s;
	char		*act_s;
	char		**passed;
	uint16_t	passed_count = 0;

	/* allocate array for passed test names (only pointers to existing strings) */
	passed = NULL;
	if (suite && suite->total > 0)
		passed = malloc(sizeof(char *) * suite->total);

	print_header("DETAILED TEST RESULTS");
	i = 0;
	while (i < suite->total)
	{
		r = &suite->results[i];
		if (r->st == TEST_PASS)
		{
			/* collect passed names, do not print details */
			if (passed)
				passed[passed_count++] = r->test_name;
		}
		else
		{
			/* print full failure details */
			printf("\n\033[1;31m[✗]\033[0m %s\n", r->test_name);
			if (r->error_msg)
				printf("  Error: %s\n", r->error_msg);
			exp_s = format_value(r->expected, r->type);
			act_s = format_value(r->actual, r->type);
			if (exp_s && act_s)
			{
				printf("  Expected (%s): %s\n", origin_to_str(r->expected_origin), exp_s);
				printf("  Actual   (%s): %s\n", origin_to_str(r->actual_origin), act_s);
			}
			free(exp_s);
			free(act_s);
		}
		i++;
	}

	/* concise checklist for passed tests */
	printf("\n\033[1;36m=== Passed tests (summary) ===\033[0m\n");
	printf("  %u passed (details suppressed)\n", passed_count);

	/* final overall result */
	if (suite->failed == 0 && suite->error == 0)
		printf("\n\033[1;32mRESULT: OK\033[0m\n");
	else
		printf("\n\033[1;31mRESULT: FAIL\033[0m\n");

	free(passed);
}
