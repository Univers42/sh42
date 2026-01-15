/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 23:32:29 by marvin            #+#    #+#             */
/*   Updated: 2026/01/09 23:32:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"
#include "ast.h"
#include "lexer.h"

// Forward declaration must appear before any use
t_ast_node unexpected(t_shell *state, t_parser *parser, t_ast_node ret, t_deque_tt *tokens);

/* Check if the current token starts a command (simple cmd, subshell, or arith) */
bool	is_command_start(t_tt tt)
{
	if (tt == TT_WORD || tt == TT_BRACE_LEFT || tt == TT_ARITH_CMD)
		return (true);
	if (tt == TT_REDIRECT_LEFT || tt == TT_REDIRECT_RIGHT)
		return (true);
	if (tt == TT_APPEND || tt == TT_HEREDOC)
		return (true);
	return (false);
}

/* Parse arithmetic command (( expr )) - called from parser when TT_ARITH_CMD seen */
t_ast_node	parse_arith_command(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_ast_node	arith_node;
	t_token		tok;
	char		*expr_start;
	int			expr_len;

	ret = (t_ast_node){.node_type = AST_COMMAND};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	
	tok = *(t_token *)deque_pop_start(&tokens->deqtok);
	
	/* Create AST_ARITH_CMD node */
	arith_node = (t_ast_node){.node_type = AST_ARITH_CMD};
	vec_init(&arith_node.children);
	arith_node.children.elem_size = sizeof(t_ast_node);
	
	/* Extract expression from (( expr )) - skip (( and )) */
	if (tok.len >= 4)
	{
		t_ast_node	expr_node;
		
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
		
		vec_push(&arith_node.children, &expr_node);
	}
	
	vec_push(&ret.children, &arith_node);
	
	(void)state;
	(void)parser;
	return (ret);
}

static bool	is_redirect_token(t_tt tt)
{
	return (tt == TT_REDIRECT_LEFT || tt == TT_REDIRECT_RIGHT
		|| tt == TT_APPEND || tt == TT_HEREDOC);
}

t_ast_node	parse_simple_command(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_token		peek;

	ret = (t_ast_node){.node_type = AST_SIMPLE_COMMAND};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	while (1)
	{
		peek = *(t_token *)deque_peek(&tokens->deqtok);
		if (peek.tt == TT_WORD)
		{
			t_token tok = *(t_token *)deque_pop_start(&tokens->deqtok);
			t_ast_node word = (t_ast_node){.node_type = AST_WORD};
			t_ast_node tnode = (t_ast_node){.node_type = AST_TOKEN, .token = tok};
			vec_init(&word.children);
			word.children.elem_size = sizeof(t_ast_node);
			vec_init(&tnode.children);
			tnode.children.elem_size = sizeof(t_ast_node);
			vec_push(&word.children, &tnode);
			vec_push(&ret.children, &word);
			continue;
		}
		if (is_redirect_token(peek.tt))
		{
			t_token redir_tok = *(t_token *)deque_pop_start(&tokens->deqtok);
			t_token next = *(t_token *)deque_peek(&tokens->deqtok);
			if (next.tt == TT_END || next.tt == TT_NEWLINE)
				return (parser->res = RES_MoreInput, ret);
			if (next.tt != TT_WORD)
				return (unexpected(state, parser, ret, tokens));
			next = *(t_token *)deque_pop_start(&tokens->deqtok);
			t_ast_node redir = (t_ast_node){.node_type = AST_REDIRECT};
			t_ast_node redir_node = (t_ast_node){.node_type = AST_TOKEN, .token = redir_tok};
			t_ast_node word = (t_ast_node){.node_type = AST_WORD};
			t_ast_node word_tok = (t_ast_node){.node_type = AST_TOKEN, .token = next};
			vec_init(&redir.children);
			redir.children.elem_size = sizeof(t_ast_node);
			vec_init(&redir_node.children);
			redir_node.children.elem_size = sizeof(t_ast_node);
			vec_init(&word.children);
			word.children.elem_size = sizeof(t_ast_node);
			vec_init(&word_tok.children);
			word_tok.children.elem_size = sizeof(t_ast_node);
			vec_push(&word.children, &word_tok);
			vec_push(&redir.children, &redir_node);
			vec_push(&redir.children, &word);
			vec_push(&ret.children, &redir);
			continue;
		}
		break;
	}
	if (ret.children.len == 0)
		return (unexpected(state, parser, ret, tokens));
	return (ret);
}

t_ast_node	parse_command(t_shell *state, t_parser *parser, t_deque_tt *tokens)
{
	t_ast_node	ret;
	t_token		peek;

	ret = (t_ast_node){.node_type = AST_COMMAND};
	vec_init(&ret.children);
	ret.children.elem_size = sizeof(t_ast_node);
	peek = *(t_token *)deque_peek(&tokens->deqtok);
	if (!is_command_start(peek.tt))
		return (unexpected(state, parser, ret, tokens));
	if (peek.tt == TT_ARITH_CMD)
	{
		t_ast_node tmp = parse_arith_command(state, parser, tokens);
		vec_push(&ret.children, &tmp);
		return (ret);
	}
	if (peek.tt == TT_BRACE_LEFT)
	{
		t_ast_node tmp = parse_subshell(state, parser, tokens);
		vec_push(&ret.children, &tmp);
		return (ret);
	}
	{
		t_ast_node tmp = parse_simple_command(state, parser, tokens);
		vec_push(&ret.children, &tmp);
	}
	return (ret);
}
