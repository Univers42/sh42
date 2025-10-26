#ifndef TEST_H
#define TEST_H

#include <stddef.h>
#include "ft_stddef.h"
#include <stdbool.h>
#include "ft_memory.h"
#include "ft_colors.h"
#include "ft_stdio.h"
#include "ft_stdlib.h"
#include "ft_string.h"
#include "ft_ctype.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

typedef enum e_test_info
{
	TEST_PASS,
	TEST_FAIL,
	TEST_ERROR,
	TEST_SKIP
} t_test_info;

typedef enum e_value_origin
{
	ORIGIN_UNKNOWN,
	ORIGIN_LITERAL,
	ORIGIN_USER_FN,
	ORIGIN_REF_FN
} t_origin;

typedef struct s_test
{
	t_test_info st;
	int line;
	const char *file;
	char *test_name; /* changed: make mutable so we can strdup and free */
	char *error_msg;
	void *expected;
	void *actual;
	t_type type;
	t_origin expected_origin; /* NEW: where expected value came from */
	t_origin actual_origin;	  /* NEW: where actual value came from */
	bool expected_owned;	  /* NEW: framework allocated expected */
	bool actual_owned;		  /* NEW: framework allocated actual */
} t_test;

typedef struct s_suite
{
	const char *name;
	uint16_t total;
	uint16_t passed;
	uint16_t failed;
	uint16_t error;
	uint16_t skipped_tests;
	t_test *results;
	uint16_t cap;
} t_suite;

typedef void *(*t_fn0)(void);
typedef void *(*t_fn1)(void *);
typedef void *(*t_fn2)(void *, void *);
typedef void *(*t_fn3)(void *, void *, void *);
typedef void *(*t_fn4)(void *, void *, void *, void *);

// custom comparator function type
typedef int (*t_comparator)(const void *a, const void *b, size_t n);

typedef struct s_case
{
	const char *description;
	void **params; // changed: dynamic params array (NULL-terminated copy)
	void *expected;
	uint8_t param_count;
	uint8_t data_size;
	t_type type;
	t_comparator cmp;
	bool failed;
} t_case;

typedef struct s_edge_case
{
	const char *description;
	void *params[4];
	uint8_t param_count;
	bool (*validator)(void *result);
} t_edge_case;

typedef struct s_fn_pair
{
	const char *name;
	union
	{
		int (*int_fn)(int);							   // for typical ctype functions: int fn(int)
		void *(*raw0)(void);						   // generic: returns pointer, 0 params
		void *(*raw1)(void *);						   // generic: returns pointer, 1 param (void*)
		void *(*raw2)(void *, void *);				   // 2 params
		void *(*raw3)(void *, void *, void *);		   // 3 params
		void *(*raw4)(void *, void *, void *, void *); // 4 params
														/* add members if you need other signatures (e.g. int(*)(void*)),
														  but rawX can be used with casting when calling via exec_fn. */
	} std_fn;
	union
	{
		int (*int_fn)(int);
		void *(*raw0)(void);
		void *(*raw1)(void *);
		void *(*raw2)(void *, void *);
		void *(*raw3)(void *, void *, void *);
		void *(*raw4)(void *, void *, void *, void *);
	} ft_fn;
	uint8_t param_count; /* number of parameters (0..4) for generic runners */
	t_type type;		 /* expected result type for comparisons */
} t_fn_pair;

t_suite *suite_create(const char *name);
void suite_destroy(t_suite *suite);
void suite_record(t_suite *suite, t_test result);

/**
 * RUNNERS
 */

void run_fn_test(t_suite *suite, const char *name, void *fn_test, void *fn_ref, t_case *tc);
void run_edge_test(t_suite *suite, const char *name, void *fn_test, t_edge_case *edge);
void run_var_test(t_suite *suite, const char *name, const void *actual, const void *expected, t_type type, size_t n);
void run_fn_batch(t_suite *suite, const char *batch_name, void *fn_test, void *fn_ref, t_case *cases, size_t count);
void run_edge_batch(t_suite *suite, const char *batch_name, void *fn_test, t_edge_case *cases, size_t count);

/**
 * reporting
 */
void suite_print_summary(const t_suite *suite);
void suite_print_detailed(const t_suite *suite);

/**
 * validators
 */

bool validate_non_null(void *result);
bool validate_null(void *result);
bool validate_positive(void *result);
bool validate_negative(void *result);
bool validate_zero(void *result);
bool validate_even(void *result);
bool validate_odd(void *result);

int cmp_values(const void *a, const void *b, t_type type, size_t n);
void *exec_fn(void *fn, uint8_t param_count, void **params);

#endif
