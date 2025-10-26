/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 16:01:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/22 16:27:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 100

t_suite *suite_create(const char *name)
{
	t_suite *suite;

	suite = malloc(sizeof(t_suite));
	if (!suite)
		return (NULL);
	suite->name = name;
	suite->total = 0;
	suite->passed = 0;
	suite->failed = 0;
	suite->error = 0;
	suite->skipped_tests = 0;
	suite->cap = INITIAL_CAP;
	suite->results = malloc(sizeof(t_test) * suite->cap);
	if (!suite->results)
	{
		free(suite);
		return (NULL);
	}
	return (suite);
}

void suite_destroy(t_suite *suite)
{
	uint16_t i;

	if (!suite)
		return;
	i = 0;
	while (i < suite->total)
	{
		/* free dynamically allocated strings stored in results */
		if (suite->results[i].error_msg)
			free(suite->results[i].error_msg);
		if (suite->results[i].test_name)
			free(suite->results[i].test_name);
		/* free owned expected/actual copies */
		if (suite->results[i].actual_owned && suite->results[i].actual)
			free(suite->results[i].actual);
		if (suite->results[i].expected_owned && suite->results[i].expected)
			free(suite->results[i].expected);
		i++;
	}
	free(suite->results);
	free(suite);
}

static void suite_update_stats(t_suite *suite, t_test_info status)
{
	if (status == TEST_PASS)
		suite->passed++;
	else if (status == TEST_FAIL)
		suite->failed++;
	else if (status == TEST_ERROR)
		suite->error++;
	else if (status == TEST_SKIP)
		suite->skipped_tests++;
}

void suite_record(t_suite *suite, t_test result)
{
	t_test *new_results;
	char *name_dup;

	if (!suite)
		return;
	/* duplicate test name to ensure it survives the caller stack */
	if (result.test_name)
		name_dup = strdup(result.test_name);
	else
		name_dup = strdup("unnamed test");
	if (!name_dup)
		name_dup = NULL;
	result.test_name = name_dup;

	if (suite->total >= suite->cap)
	{
		suite->cap *= 2;
		new_results = malloc(sizeof(t_test) * suite->cap);
		if (!new_results)
			return;
		memcpy(new_results, suite->results, sizeof(t_test) * suite->total);
		free(suite->results);
		suite->results = new_results;
	}
	suite->results[suite->total++] = result;
	suite_update_stats(suite, result.st);
}
