/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_proc_sub_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:49:25 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/25 20:50:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

int	proc_sub_get_depth_delta(t_token curr)
{
	if (curr.tt == TT_ARITH_START)
		return (2);
	if (curr.tt == TT_BRACE_LEFT
		|| curr.tt == TT_PROC_SUB_IN
		|| curr.tt == TT_PROC_SUB_OUT)
		return (1);
	else if (curr.tt == TT_BRACE_RIGHT)
		return (-1);
	return (0);
}

void	proc_sub_consume_and_append(t_deque_tt *tokens,
												t_string *cmd_str,
												t_token curr)
{
	deque_pop_start(&tokens->deqtok);
	if (cmd_str->len > 0)
		vec_push_char(cmd_str, ' ');
	if (curr.tt == TT_ARITH_START)
		vec_push_str(cmd_str, "( (");
	else if (curr.tt == TT_BRACE_LEFT)
		vec_push_char(cmd_str, '(');
	else
		vec_push_nstr(cmd_str, curr.start, curr.len);
	if (curr.tt == TT_ARITH_START || curr.tt == TT_BRACE_LEFT)
		vec_push_char(cmd_str, ' ');
}
