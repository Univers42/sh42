/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:31:57 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/25 22:44:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_private.h"

bool	node_is_groupable(t_ast_node *node)
{
	t_ast_node	*first;

	if (node->node_type == AST_SIMPLE_COMMAND)
		return (true);
	if (node->node_type == AST_COMMAND && node->children.len > 0
		&& node->children.ctx)
	{
		first = &((t_ast_node *)node->children.ctx)[0];
		if (first->node_type == AST_SIMPLE_COMMAND)
			return (true);
	}
	return (false);
}

bool	should_skip_node(t_ast_node *node)
{
	if (!node)
		return (true);
	if (node->node_type == AST_PROC_SUB)
		return (true);
	if (node->node_type == AST_TOKEN)
		return (true);
	if (node->node_type == AST_WORD)
		return (true);
	return (false);
}

void	recurse_non_redirect_child(t_shell *state,
									t_ast_node *node,
									size_t *idx)
{
	t_ast_node	*child;

	if (!node->children.ctx || *idx >= node->children.len)
	{
		(*idx)++;
		return ;
	}
	child = &((t_ast_node *)node->children.ctx)[*idx];
	if (!should_skip_node(child))
		gather_heredocs(state, child, false);
	(*idx)++;
}

void	handle_non_groupable_redirect(t_shell *state,
									t_ast_node *node,
									size_t *idx,
									bool in_pipeline)
{
	t_ast_node	*child;

	if (!node->children.ctx || *idx >= node->children.len)
	{
		(*idx)++;
		return ;
	}
	child = &((t_ast_node *)node->children.ctx)[*idx];
	if (child->node_type == AST_REDIRECT)
		gather_heredoc(state, child, in_pipeline);
	(*idx)++;
}

void	handle_grouped_redirects(t_shell *state,
								t_ast_node *node,
								size_t start)
{
	size_t		end;
	t_ast_node	*children;

	children = (t_ast_node *)node->children.ctx;
	end = start;
	while (end < node->children.len && children[end].node_type == AST_REDIRECT)
		end++;
	if (end > start)
		process_redirect_group(state, node, start, end);
}
