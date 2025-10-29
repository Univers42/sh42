/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_perfect_hash.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 12:55:43 by syzygy            #+#    #+#             */
/*   Updated: 2025/10/29 20:43:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// __attribute__((weak))
// t_token_type	identifier_type(t_scanner *scan)
// {
// 	const t_keyword_entry	*hash_table;
// 	const t_keyword_entry	*entry;
// 	int						hash;
// 	int						length;

// 	hash_table = map_keywords();
// 	length = (int)(scan->current - scan->start);
// 	hash = keyword_hash(scan->start, length);
// 	entry = &hash_table[hash];
// 	if (entry && entry->length
// 		&& memcmp(scan->start, entry->keyword, (size_t)length) == 0)
// 		return (entry->type);
// 	return (TOKEN_IDENTIFIER);
// }
