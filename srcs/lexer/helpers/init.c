/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:57:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/26 17:57:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_scanner global_scanner;

t_scanner   *init_scanner(const char *source)
{
	global_scanner.start = source;
	global_scanner.current = source;
	global_scanner.line = 1;
	return (&global_scanner);
}
