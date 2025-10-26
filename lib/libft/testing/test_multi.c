/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_multi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:22:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/22 16:10:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// I'm not sure but maybe optional this one
static void *memcmp_wrapper(void *s1, void *s2, void *n)
{
    return ((void *)(long)ft_memcmp(s1, s2, (size_t)(long)n));
}

void test_memcmp(t_suite *suite)
{
    t_case cases[3];
    char buf1[] = "Hello";
    char buf2[] = "Hello";
    char buf3[] = "World";

    void *params0[] = {buf1, buf2, (void *)(long)5};
    void *params1[] = {buf1, buf3, (void *)(long)5};

    cases[0] = (t_case){
        .description = "identical strings",
        .params = params0,
        .param_count = 3,
        .data_size = sizeof(int),
        .type = TYPE_INT,
        .cmp = NULL,
        .failed = FALSE};

    cases[1] = (t_case){
        .description = "different strings",
        .params = params1,
        .param_count = 3,
        .data_size = sizeof(int),
        .type = TYPE_INT,
        .cmp = NULL,
        .failed = FALSE};

    run_fn_batch(suite, "memcmp", (void *)memcmp_wrapper,
                 (void *)memcmp_wrapper, cases, 2);
}

int main(void)
{
    t_suite *suite;

    suite = suite_create("Multi-parameter function tests");
    if (!suite)
        return (1);
    test_memcmp(suite);
    suite_print_summary(suite);
    suite_destroy(suite);
    return 0;
}
