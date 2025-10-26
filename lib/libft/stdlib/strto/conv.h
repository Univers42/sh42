/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:00:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 00:51:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONV_H
#define CONV_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include "ft_stddef.h"

// Forward-declare context for the function pointer type
struct s_conv_ctx;
typedef void (*t_fn_state)(struct s_conv_ctx *);

// Conversion context (state machine data)
typedef struct s_conv_ctx
{
    const char *ptr;
    const char *start;
    char **endptr;
    int base;
    t_type type;
    t_type_info type_info;
    uint64_t uval;
    bool negative;
    bool any_digit;
    t_state state;
} t_conv_ctx;

typedef struct
{
    t_type type;
    unsigned long long max_val;
    long long min_val;
    bool is_signed;
} type_info_entry;

// Accessor for the static type info table
static inline const type_info_entry *get_type_info_table(size_t *count)
{
    static const type_info_entry table[] = {
        {TYPE_UCHAR, UCHAR_MAX, 0, false},
        {TYPE_CHAR, CHAR_MAX, CHAR_MIN, true},
        {TYPE_USHORT, USHRT_MAX, 0, false},
        {TYPE_SHORT, SHRT_MAX, SHRT_MIN, true},
        {TYPE_UINT, UINT_MAX, 0, false},
        {TYPE_INT, INT_MAX, INT_MIN, true},
        {TYPE_ULONG, ULONG_MAX, 0, false},
        {TYPE_LONG, LONG_MAX, LONG_MIN, true},
        {TYPE_ULLONG, ULLONG_MAX, 0, false},
        {TYPE_LLONG, LLONG_MAX, LLONG_MIN, true},
        {TYPE_UINT8, UINT8_MAX, 0, false},
        {TYPE_INT8, INT8_MAX, INT8_MIN, true},
        {TYPE_UINT16, UINT16_MAX, 0, false},
        {TYPE_INT16, INT16_MAX, INT16_MIN, true},
        {TYPE_UINT32, UINT_MAX, 0, false},
        {TYPE_INT32, INT_MAX, INT_MIN, true},
        {TYPE_UINT64, ULLONG_MAX, 0, false},
        {TYPE_INT64, LLONG_MAX, LLONG_MIN, true}};

    if (count)
        *count = sizeof(table) / sizeof(table[0]);
    return (table);
}

int64_t ft_strto(const char *nptr, char **endptr, int base,
                 t_type type);
int8_t ft_strto_i8(const char *nptr, char **endptr, int base);
uint16_t ft_strto_u16(const char *nptr, char **endptr, int base);
int16_t ft_strto_i16(const char *nptr, char **endptr, int base);
uint32_t ft_strto_u32(const char *nptr, char **endptr, int base);
int32_t ft_strto_i32(const char *nptr, char **endptr, int base);
unsigned int ft_strto_uint(const char *nptr, char **endptr, int base);
int ft_strto_int(const char *nptr, char **endptr, int base);
unsigned long ft_strto_ulong(const char *nptr, char **endptr, int base);
long ft_strto_long(const char *nptr, char **endptr, int base);
uint8_t ft_strto_u8(const char *nptr, char **endptr, int base);
int64_t ft_strto64(const char *nptr, char **endptr, int base);
unsigned char ft_strto_uchar(const char *nptr, char **endptr, int base);
signed char ft_strto_char(const char *nptr, char **endptr, int base);
unsigned short ft_strto_ushort(const char *nptr, char **endptr, int base);
short ft_strto_short(const char *nptr, char **endptr, int base);
uint64_t ft_strtoull(const char *nptr, char **endptr, int base);
uint64_t ft_strtou64(const char *nptr, char **endptr, int base);

// Existing state handlers
void state_whitespace(t_conv_ctx *ctx);
void state_sign(t_conv_ctx *ctx);
void state_base_prefix(t_conv_ctx *ctx);
void state_digits(t_conv_ctx *ctx);
void state_overflow(t_conv_ctx *ctx);

// Lookup for state handler
t_fn_state lookup_state_fn(t_state state);

// Helpers
void init_conv_ctx(t_conv_ctx *ctx, const char *nptr, char **endptr,
                   int base, t_type type);
int char_to_digit(char c, int base);

#endif
