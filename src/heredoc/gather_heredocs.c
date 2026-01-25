/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 20:52:16 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/25 22:44:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc_private.h"

bool	node_is_groupable(t_ast_node *node);
bool	should_skip_node(t_ast_node *node);
void	recurse_non_redirect_child(t_shell *state,
			t_ast_node *node,
			size_t *idx);
void	handle_non_groupable_redirect(t_shell *state,
			t_ast_node *node,
			size_t *idx,
			bool in_pipeline);
void	handle_grouped_redirects(t_shell *state,
			t_ast_node *node,
			size_t start);

static int	process_leaf_node(t_shell *state, t_ast_node *node,
							bool in_pipeline)
{
	if (!node->children.ctx || node->children.len == 0)
	{
		if (node->node_type == AST_REDIRECT)
			gather_heredoc(state, node, in_pipeline);
		return (0);
	}
	return (1);
}

static void	skip_redirect_sequence(size_t *i, t_ast_node *node)
{
	t_ast_node	*children;

	children = (t_ast_node *)node->children.ctx;
	while (*i < node->children.len && children[*i].node_type == AST_REDIRECT)
		(*i)++;
}

static void	process_redirect_child(t_shell *state, t_ast_node *node,
								size_t *i, bool in_pipeline)
{
	if (!node_is_groupable(node))
	{
		handle_non_groupable_redirect(state, node, i, in_pipeline);
		return ;
	}
	handle_grouped_redirects(state, node, *i);
	skip_redirect_sequence(i, node);
}

int	gather_heredocs(t_shell *state, t_ast_node *node, bool in_pipeline)
{
	size_t		i;
	t_ast_node	*child;
	bool		is_pipe;

	if (!node || !node->children.ctx)
		return (0);
	if (!process_leaf_node(state, node, in_pipeline))
		return (0);
	is_pipe = (node->node_type == AST_COMMAND_PIPELINE);
	i = 0;
	while (i < node->children.len)
	{
		child = &((t_ast_node *)node->children.ctx)[i];
		if (child->node_type == AST_REDIRECT)
			process_redirect_child(state, node, &i, in_pipeline || is_pipe);
		else if (!should_skip_node(child))
		{
			gather_heredocs(state, child, in_pipeline || is_pipe);
			i++;
		}
		else
			i++;
	}
	return (0);
}
