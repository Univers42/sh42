/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 23:32:29 by marvin            #+#    #+#             */
/*   Updated: 2026/01/09 23:32:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"
#include "shell.h"
# include "parser.h"
# include "ast.h"
# include "decomposer.h"
# include "sys.h"

t_ast_node unexpected(t_shell *state, t_parser *parser, t_ast_node ret, t_deque_tt *tokens);

bool	is_simple_cmd_token(t_tt tt)
{
	if (tt == TT_REDIRECT_LEFT || tt == TT_ARITH_CMD)
		return (true);
	return (false);
}

t_ast_node	create_subtoken_node(t_token t, int offset, int end_offset, t_tt tt)
{
	t_ast_node	ret;

	(void)t;
	(void)offset;
	(void)end_offset;
	ret = (t_ast_node){.token = {
		.tt = tt,
	},
		.node_type = AST_TOKEN};
	/* ensure children vector is usable for later pushes */
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	return (ret);
}

/* Parse arithmetic command (( expr )) into AST_ARITH_CMD node */
t_ast_node	parse_arith_cmd(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_token		tok;
	t_ast_node	expr_node;
	char		*expr_start;
	int			expr_len;

	ret = (t_ast_node){.node_type = AST_ARITH_CMD};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	
	tok = *(t_token *)deque_pop_start(&tokens->deqtok);
	
	/* Extract expression from (( expr )) - skip (( and )) */
	if (tok.len < 4)
	{
		parser->res = RES_FatalError;
		ft_eprintf("%s: syntax error in arithmetic expression\n", state->context);
		return (ret);
	}
	expr_start = tok.start + 2;  /* skip (( */
	expr_len = tok.len - 4;      /* remove (( and )) */
	
	/* Create a token node containing just the expression */
	expr_node = (t_ast_node){.node_type = AST_TOKEN};
	expr_node.token = (t_token){
		.tt = TT_WORD,
		.start = expr_start,
		.len = expr_len,
		.allocated = false
	};
	vec_init(&expr_node.children);
	expr_node.children.elem_size = sizeof(t_ast_node);
	
	vec_push(&ret.children, &expr_node);
	
	return (ret);
}

t_ast_node	parse_subshell(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_token		peek_t;

	ret = (t_ast_node){.node_type = AST_SUBSHELL};
	/* init children storage */
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	{ int val = TT_BRACE_LEFT; vec_push(&parser->parse_stack, &val); }
	peek_t = *(t_token *)deque_peek(&tokens->deqtok);
	if (peek_t.tt != TT_BRACE_LEFT)
		return (unexpected(state, parser, ret, tokens));
	(void)deque_pop_start(&tokens->deqtok);
	{
		t_ast_node tmp_node = parse_compound_list(state, parser, tokens);
		/* ensure tmp_node children vector is initialized (parse_compound_list does this) */
		vec_push(&ret.children, &tmp_node);
	}
	if (parser->res != RES_OK)
		return (ret);
	peek_t = *(t_token *)deque_peek(&tokens->deqtok);
	if (peek_t.tt != TT_BRACE_RIGHT)
		return (unexpected(state, parser, ret, tokens));
	(void)deque_pop_start(&tokens->deqtok);
	vec_pop(&parser->parse_stack);
	return (ret);
}

t_ast_node	parse_pipeline(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;

	ret = (t_ast_node){.node_type = AST_COMMAND_PIPELINE};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	{
		t_ast_node tmp_node = parse_command(state, parser, tokens);
		vec_push(&ret.children, &tmp_node);
	}
	if (parser->res != RES_OK)
		return (ret);
	{ int val = TT_PIPE; vec_push(&parser->parse_stack, &val); }
	while ((*(t_token *)deque_peek(&tokens->deqtok)).tt == TT_PIPE)
	{
		(void)deque_pop_start(&tokens->deqtok);
		while ((*(t_token *)deque_peek(&tokens->deqtok)).tt == TT_NEWLINE)
			(void)deque_pop_start(&tokens->deqtok);
		if ((*(t_token *)deque_peek(&tokens->deqtok)).tt == TT_END)
			return (parser->res = RES_MoreInput, ret);
		{
			t_ast_node tmp_node = parse_command(state, parser, tokens);
			vec_push(&ret.children, &tmp_node);
		}
		if (parser->res != RES_OK)
			return (ret);
	}
	vec_pop(&parser->parse_stack);
	return (ret);
}

t_ast_node	parse_tokens(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_tt		tt;
	t_ast_node	ret;
	t_token		tmp;

	parser->res = RES_OK;
	ret = parse_simple_list(state, parser, tokens);
	if (parser->res == RES_OK)
	{
		tmp = *(t_token *)deque_pop_start(&tokens->deqtok);
		tt = tmp.tt;
		if (tt != TT_END)
			ft_printf("Got token: %s\n", tt_to_str(tt));
		ft_assert(tt == TT_END);
		ft_assert(tokens->deqtok.len == 0);
		reparse_words(&ret);
		reparse_assignment_words(&ret);
	}
	if (PRINT_AST)
		print_ast_dot(state, ret);
	return (ret);
}
