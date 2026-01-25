/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 19:10:18 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/25 22:40:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

static bool	is_valid_redirect_target(t_tt tt)
{
	return (tt == TT_WORD || tt == TT_SQWORD || tt == TT_DQWORD
		|| tt == TT_ENVVAR || tt == TT_DQENVVAR
		|| tt == TT_PROC_SUB_IN || tt == TT_PROC_SUB_OUT);
}

static bool	is_valid_heredoc_delimiter(t_tt tt)
{
	return (tt == TT_WORD || tt == TT_SQWORD || tt == TT_DQWORD
		|| tt == TT_ENVVAR || tt == TT_DQENVVAR);
}

t_ast_node	parse_redirect(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_ast_node	tmp_node;
	t_token		t;
	t_token		next;

	ret = (t_ast_node){.node_type = AST_REDIRECT};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	t = *(t_token *)deque_pop_start(&tokens->deqtok);
	if (!is_redirect(t.tt))
		return (unexpected(state, parser, ret, tokens));
	push_token_child(&ret, t);
	next = *(t_token *)deque_peek(&tokens->deqtok);
	if (t.tt == TT_HEREDOC)
	{
		if (!is_valid_heredoc_delimiter(next.tt))
			return (unexpected(state, parser, ret, tokens));
		tmp_node = parse_word(tokens);
		vec_push(&ret.children, &tmp_node);
		return (ret);
	}
	if (next.tt == TT_PROC_SUB_IN || next.tt == TT_PROC_SUB_OUT)
	{
		tmp_node = parse_proc_sub(state, parser, tokens);
		vec_push(&ret.children, &tmp_node);
		return (ret);
	}
	if (!is_valid_redirect_target(next.tt))
		return (unexpected(state, parser, ret, tokens));
	tmp_node = parse_word(tokens);
	vec_push(&ret.children, &tmp_node);
	return (ret);
}
