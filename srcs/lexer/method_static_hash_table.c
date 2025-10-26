/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_static_hash_table.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:10:11 by syzygy            #+#    #+#             */
/*   Updated: 2025/10/26 13:17:25 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

__attribute__((weak))
t_token_type	identifier_type(t_scanner *scan)
{
	int	length;
	const	t_keyword_entry	*kw = map_keywords();

	length = (int)(scan->current - scan->start);
	while (kw->keyword != NULL)
	{
		if (kw->length == length
			&& ft_memcmp(scan->start, kw->keyword, length) == 0)
				return (kw->type);
		kw++;
	}
}