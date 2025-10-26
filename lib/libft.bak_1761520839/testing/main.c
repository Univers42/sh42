#include "test.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static void test_strlen_cmp(t_suite *suite)
{
    const char *samples[] = {
        "",
        "a",
        "hello world",
        "The quick brown fox jumps over the lazy dog",
        "0123456789",
        NULL};
    size_t i = 0;
    while (samples[i])
    {
        size_t r1 = ft_strlen(samples[i]);
        size_t r2 = strlen(samples[i]);
        /* compare as unsigned long */
        run_var_test(suite, "strlen", &r1, &r2, TYPE_ULONG, sizeof(size_t));
        i++;
    }
}

static void test_ctype_cmp(t_suite *suite)
{
    const int chars[] = {'0', '5', '9', 'A', 'Z', 'a', 'z', ' ', '\n', '\t', '@', '#', 0, -1};
    size_t i = 0;
    while (chars[i] != -1)
    {
        int c = chars[i];
        int r_ft_isdigit = ft_isdigit(c);
        int r_isdigit = isdigit(c);
        char namebuf[64];

        snprintf(namebuf, sizeof(namebuf), "isdigit(%d)", c);
        run_var_test(suite, namebuf, &r_ft_isdigit, &r_isdigit, TYPE_BOOL, sizeof(int));

        int r_ft_isalpha = ft_isalpha(c);
        int r_isalpha = isalpha(c);
        snprintf(namebuf, sizeof(namebuf), "isalpha(%d)", c);
        run_var_test(suite, namebuf, &r_ft_isalpha, &r_isalpha, TYPE_BOOL, sizeof(int));

        int r_ft_isalnum = ft_isalnum(c);
        int r_isalnum = isalnum(c);
        snprintf(namebuf, sizeof(namebuf), "isalnum(%d)", c);
        run_var_test(suite, namebuf, &r_ft_isalnum, &r_isalnum, TYPE_BOOL, sizeof(int));

        i++;
    }
}

static void test_memcmp_outputs(t_suite *suite)
{
    char a1[] = "Hello";
    char a2[] = "Hello";
    char b[] = "World";

    int r1 = ft_memcmp(a1, a2, 5);
    int r2 = memcmp(a1, a2, 5);
    /* compare only sign: -1 / 0 / 1 */
    {
        int s1 = (r1 > 0) - (r1 < 0);
        int s2 = (r2 > 0) - (r2 < 0);
        run_var_test(suite, "ft_memcmp equal", &s1, &s2, TYPE_INT, sizeof(int));
    }

    r1 = ft_memcmp(a1, b, 5);
    r2 = memcmp(a1, b, 5);
    {
        int s1 = (r1 > 0) - (r1 < 0);
        int s2 = (r2 > 0) - (r2 < 0);
        run_var_test(suite, "ft_memcmp diff", &s1, &s2, TYPE_INT, sizeof(int));
    }
}

int main(void)
{
    t_suite *suite = suite_create("Framework Demo - var comparisons");

    test_strlen_cmp(suite);
    test_ctype_cmp(suite);
    test_memcmp_outputs(suite);

    suite_print_summary(suite);
    suite_print_detailed(suite);
    suite_destroy(suite);
    return 0;
}
