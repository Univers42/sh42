/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:24:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 17:29:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "version_print.h"
#include "parser.h"
#include "parser_private.h"
#include "writer.h"
#include <stdarg.h>
#include <limits.h>

/* sprintf-style: unbounded; caller must ensure dst has enough space.
   Implemented via vsnprintf with a very large capacity.
   we need to give a huge capacity LONG_MAX
*/
int	ft_vsprintf(char *dst, const char *format, va_list *ap)
{
	return (ft_vsnprintf(dst, (size_t)-1, format, ap));
}

int	ft_sprintf(char *dst, const char *format, ...)
{
	va_list	ap;
	int		return_value;

	va_start(ap, format);
	return_value = ft_vsnprintf(dst, (size_t)-1, format, &ap);
	va_end(ap);
	return (return_value);
}
