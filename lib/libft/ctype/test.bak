#include "test.h"
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

/* use shared t_fn_pair from ../include/test.h */
static t_fn_pair *ctype_pairs(size_t *count)
{
	static t_fn_pair pairs[] = {
		{"isdigit", .std_fn.int_fn = isdigit, .ft_fn.int_fn = ft_isdigit, .param_count = 1, .type = TYPE_BOOL},
		{"isalnum", .std_fn.int_fn = isalnum, .ft_fn.int_fn = ft_isalnum, .param_count = 1, .type = TYPE_BOOL},
		{"isalpha", .std_fn.int_fn = isalpha, .ft_fn.int_fn = ft_isalpha, .param_count = 1, .type = TYPE_BOOL},
		{"isblank", .std_fn.int_fn = isblank, .ft_fn.int_fn = ft_isblank, .param_count = 1, .type = TYPE_BOOL},
		{"isgraph", .std_fn.int_fn = isgraph, .ft_fn.int_fn = ft_isgraph, .param_count = 1, .type = TYPE_BOOL},
		{"islower", .std_fn.int_fn = islower, .ft_fn.int_fn = ft_islower, .param_count = 1, .type = TYPE_BOOL},
		{"isupper", .std_fn.int_fn = isupper, .ft_fn.int_fn = ft_isupper, .param_count = 1, .type = TYPE_BOOL},
		{"isprint", .std_fn.int_fn = isprint, .ft_fn.int_fn = ft_isprint, .param_count = 1, .type = TYPE_BOOL},
		{"isspace", .std_fn.int_fn = isspace, .ft_fn.int_fn = ft_isspace, .param_count = 1, .type = TYPE_BOOL},
		{"isxdigit", .std_fn.int_fn = isxdigit, .ft_fn.int_fn = ft_isxdigit, .param_count = 1, .type = TYPE_BOOL},
		{"tolower", .std_fn.int_fn = tolower, .ft_fn.int_fn = ft_tolower, .param_count = 1, .type = TYPE_INT},
		{"toupper", .std_fn.int_fn = toupper, .ft_fn.int_fn = ft_toupper, .param_count = 1, .type = TYPE_INT},
	};
	if (count)
		*count = sizeof(pairs) / sizeof(*pairs);
	return pairs;
}

static void test_ctype_cmp(t_suite *suite)
{
	size_t npairs, i, c;
	t_fn_pair *pairs = ctype_pairs(&npairs);
	char namebuf[64];

	/* Read optional runtime controls */
	const char *env_max = getenv("TEST_MAX");
	size_t max_c = 256;
	if (env_max)
	{
		long v = strtol(env_max, NULL, 10);
		if (v > 0 && v <= 256)
			max_c = (size_t)v;
	}
	const char *env_stop = getenv("STOP_ON_FAIL");
	int stop_on_fail = (env_stop && env_stop[0] == '1');

	for (i = 0; i < npairs; ++i)
	{
		for (c = 0; c < max_c; ++c)
		{
			int std_res = pairs[i].std_fn.int_fn((int)c);
			int ft_res = pairs[i].ft_fn.int_fn((int)c);
			ft_snprintf(namebuf, sizeof(namebuf), "%s(%d)", pairs[i].name, (int)c);

			/* quick mismatch check so we can stop early when requested */
			int mismatch = 0;
			if (pairs[i].type == TYPE_BOOL)
			{
				mismatch = (!!std_res != !!ft_res);
			}
			else /* integer comparison for conversion functions */
				mismatch = (std_res != ft_res);
			run_var_test(suite, namebuf, &ft_res, &std_res, pairs[i].type, sizeof(int));
			if (stop_on_fail && mismatch)
				return;
		}
	}
}

int main(void)
{
	t_suite *suite;

	suite = suite_create("ctype");
	test_ctype_cmp(suite);
	suite_print_summary(suite);
	suite_destroy(suite);
}