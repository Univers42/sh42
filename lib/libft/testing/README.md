# Test Framework Usage Guide
## Overview

> A **norminette-compliant**, general-purpose C testing framework that integrates my libft.
---

## Core concepts
### 1. Test Contexts
The framework supports 3 main testing contexts:
#### Function to Function comparison:
Compare our impelementatoin against a reference that already exist

```c
run_fn_test(suite, "test_name", my_fn, std_fn, &test_case);
```
#### Edge case validation
When function to function is not enough or impossible, we use custom validators:
```c
run_edge_test(suite, "test_name", my_fn, &edge_case);
```
#### Context 3: Direct Variable Comparison
Compare variables without execution functions

```c
run_var_test(suite, "test_name", &actulal, &expected, TYPE_INT, sizeof(int))
```

---
# Quick start:
```c
#include "test_api.h"

int     main(void)
{
	t_suite *suite = suite_create("My test");

	suite_print_summary(suite);
	suite_destroy(suite);
	return (0);
}
```

## Step 2: Define Test Cases

### For function comparison
```c
t_case	test;
test.description = "test with value 5";
test.params[0] = (void *)(long)5;
test.expected = NULL;
test.param_count = 1;
test.data_size = sizeof(int);
test.cmp = NULL;
test.failed = FALSE;
run_fn_test(suite, "my_test", my_fn, std_fn, &test);
```

### For Edge cases

```c
t_edge_case edge;

edge.description = "Result should be positive";
edge.params[0] = (void *)(long)-5;
edge.param_count = 1;
edge.validator = validate_positive;

run_edge_test(suite, "edge_test", my_abs, &edge);
```

## Built-in validators:

```c
validate_non_null(void *result)  // result != NULL
validate_null(void *result)      // result == NULL
validate_positive(void *result)  // *(int*)result > 0
validate_negative(void *result)  // *(int*)result < 0
validate_zero(void *result)      // *(int*)result == 0
validate_even(void *result)      // *(int*)result % 2 == 0
validate_odd(void *result)       // *(int*)result % 2 != 0
```

you can add easily a custom validator following this pattern
```c
bool my_validator(void *result)
{
    int val = *(int *)result;
    return (val >= 0 && val <= 100);
}
```

### Different kind of pattern:

#### Pattern 1
```c
static void test_isdigit(t_suite *suite)
{
    t_case cases[256];
    size_t i;
    
    i = 0;
    while (i < 256)
    {
        cases[i].params[0] = (void *)(long)i;
        cases[i].param_count = 1;
        cases[i].type = TYPE_BOOL;  // Compare truthiness
        cases[i].data_size = sizeof(int);
        cases[i].cmp = NULL;
        cases[i].failed = FALSE;
        i++;
    }
    
    run_fn_batch(suite, "isdigit", 
        (void *)ft_isdigit, (void *)isdigit, cases, 256);
}
```

#### Pattern 2
```c
// Wrapper for functions that return non-void*
static void *memcmp_wrapper(void *s1, void *s2, void *n)
{
    return ((void *)(long)memcmp(s1, s2, (size_t)(long)n));
}

static void test_memcmp(t_suite *suite)
{
    t_case test;
    char buf1[] = "Hello";
    char buf2[] = "Hello";
    
    test.params[0] = buf1;
    test.params[1] = buf2;
    test.params[2] = (void *)(long)5;
    test.param_count = 3;
    test.type = TYPE_INT;
    test.data_size = sizeof(int);
    
    run_fn_test(suite, "memcmp_test",
        (void *)memcmp_wrapper, (void *)memcmp_wrapper, &test);
}
```


#### Pattern 3

```c
static void test_abs_edges(t_suite *suite)
{
    t_edge_case cases[3];
    
    cases[0].description = "abs(0) == 0";
    cases[0].params[0] = (void *)(long)0;
    cases[0].param_count = 1;
    cases[0].validator = validate_zero;
    
    cases[1].description = "abs(-5) > 0";
    cases[1].params[0] = (void *)(long)-5;
    cases[1].param_count = 1;
    cases[1].validator = validate_positive;
    
    cases[2].description = "abs doesn't crash on INT_MIN";
    cases[2].params[0] = (void *)(long)(-2147483648);
    cases[2].param_count = 1;
    cases[2].validator = NULL;  // Just check no crash
    
    run_edge_batch(suite, "abs", (void *)my_abs, cases, 3);
}
```

#### Pattern 4

```c
static void test_variables(t_suite *suite)
{
    int x = 42;
    int y = 42;
    float pi = 3.14159f;
    float approx_pi = 3.14159f;
    const char *s1 = "hello";
    const char *s2 = "hello";
    
    // Test integers
    run_var_test(suite, "x == y", &x, &y, TYPE_INT, sizeof(int));
    
    // Test floats (automatic epsilon comparison)
    run_var_test(suite, "pi == approx_pi", 
        &pi, &approx_pi, TYPE_FLOAT, sizeof(float));
    
    // Test strings
    run_var_test(suite, "s1 == s2", s1, s2, TYPE_STRING, 0);
}
```

### Pattern 5

```c
typedef struct s_point
{
    int x;
    int y;
} t_point;

static void test_structs(t_suite *suite)
{
    t_point p1 = {10, 20};
    t_point p2 = {10, 20};
    
    run_var_test(
```