/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:54 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:57 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include "ds.h"
#include "parser.h"

bool	is_empty_token_list(t_deque *tokens)
{
	t_token_type	first_token;

	if (tokens->len < 2)
		return (true);
	first_token = (t_token_type)deque_idx(tokens, 0);
	if (tokens->len == 2 && first_token == TOKEN_NEWLINE)
		return (true);
	return (false);
}
