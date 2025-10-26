/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stddef.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:37:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 00:16:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STDDEF_H
#define FT_STDDEF_H
#include <stdint.h>
#include <sys/time.h>
#include <stdbool.h>

typedef void *t_addr;
typedef void *t_ptr;

typedef long t_ssize;
typedef unsigned long int t_size;
typedef char *t_string;
// maps
typedef char **t_strings;
typedef unsigned char t_uint8;
typedef struct sigaction t_sigaction;
// used for inter-thread or signal communication
typedef volatile int t_sig_atomic;
typedef int *t_array;
// Good name for char** typedef:
typedef float t_weight;
typedef double t_prec;
typedef int t_qty;
typedef int t_flag;
typedef uint64_t t_time;
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define STDIN_FILENO 0
// # define INT_MAX 2147483647
// # define INT_MIN (-2147483648)
// # define NULL ((void *)0)
#define SUCCESS 0
#define FAILURE 1
#define ERROR 1
#define OK 0

typedef enum s_severity
{
	SEV_INFO = 100,
	SEV_WARNING = 200,
	SEV_ERROR = 300
} t_severity;

typedef enum e_state
{
	ST_FILE_NOT_FOUND = -1,
	ST_OK = 0,
	ST_DONE = 0,
	ST_INIT = 0,
	ST_FOUND_NL,
	ST_INFO_BASE = SEV_INFO,
	ST_WHITESPACE,
	ST_SIGN,
	ST_BASE_PREFIX,
	ST_DIGITS,
	ST_FILLED,
	ST_SCANNING,
	ST_RESET_ALLOC,
	ST_RESET_PTR,
	ST_EOF,
	ST_WARNING_BASE = SEV_WARNING,
	ST_OVERFLOW,
	ST_ERR_BASE = SEV_ERROR,
	ST_ERR_ALLOC,
	ST_ERR_FATAL
} t_state;

typedef enum e_bool
{
	FALSE,
	TRUE
} t_bool;

typedef enum e_order
{
	ASCENDING,
	DESCENDING
} t_order;

typedef enum e_sign
{
	NEGATIVE = -1,
	ZERO = 0,
	POSITIVE = 1
} t_sign;

typedef enum e_side
{
	LEFT,
	RIGHT
} t_side;

typedef enum e_parity
{
	EVEN,
	ODD
} t_parity;

typedef enum e_overflow
{
	NO_OVERFLOW,
	OVERFLOW,
	UNDERFLOW
} t_overflow;

typedef enum e_endianness
{
	FT_LITTLE_ENDIAN,
	FT_BIG_ENDIAN
} t_endianness;

typedef enum e_option
{
	OPTION_OFF,
	OPTION_ON
} t_option;

typedef enum e_mode
{
	MODE_READ,
	MODE_WRITE,
	MODE_APPEND
} t_mode;

typedef enum e_type
{
	TYPE_UCHAR,
	TYPE_CHAR,
	TYPE_USHORT,
	TYPE_SHORT,
	TYPE_UINT,
	TYPE_INT,
	TYPE_ULONG,
	TYPE_LONG,
	TYPE_LLONG,
	TYPE_ULLONG,
	TYPE_UINT8,
	TYPE_INT8,
	TYPE_UINT16,
	TYPE_INT16,
	TYPE_UINT32,
	TYPE_INT32,
	TYPE_UINT64,
	TYPE_INT64,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_BOOL,
	TYPE_DOUBLE,
	TYPE_VOID,
	TYPE_PTR,
	TYPE_FLOAT_PTR,
	TYPE_DOUBLE_PTR
} t_type;

typedef enum e_direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} t_direction;

typedef struct s_type_info
{
	unsigned long long max_val;
	long long min_val;
	bool is_signed;
} t_type_info;

typedef enum e_alignment
{
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
} t_alignment;

typedef enum e_resize
{
	RESIZE_INCREASE,
	RESIZE_DECREASE,
	RESIZE_MAINTAIN
} t_resize;

typedef enum e_filter
{
	FILTER_NONE,
	FILTER_LOW_PASS,
	FILTER_HIGH_PASS,
	FILTER_BAND_PASS,
	FILTER_BAND_STOP
} t_filter;

typedef enum e_sort_algorithm
{
	SORT_BUBBLE,
	SORT_SELECTION,
	SORT_INSERTION,
	SORT_MERGE,
	SORT_QUICK,
	SORT_HEAP
} t_sort_algorithm;

typedef enum e_search_algorithm
{
	SEARCH_LINEAR,
	SEARCH_BINARY,
	SEARCH_JUMP,
	SEARCH_INTERPOLATION
} t_search_algorithm;

#endif
