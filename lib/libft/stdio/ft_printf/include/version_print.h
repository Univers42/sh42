/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version_print.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:15:21 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 17:31:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERSION_PRINT_H
# define VERSION_PRINT_H

#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

int ft_vfprintf(int fd, const char *format, va_list *ap);
int ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap);
int	ft_vsprintf(char *dst, const char *format, va_list *ap);

# endif
