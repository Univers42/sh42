/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 12:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/01/15 12:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdlib.h>

/*
** Custom history search function for up arrow.
** If there's text before the cursor, search for history entries
** that start with that prefix. Otherwise, just go to previous entry.
*/
static int	history_search_backward_prefix(int count, int key)
{
	int		ret;
	char	*prefix;
	int		prefix_len;

	(void)key;
	(void)count;
	/* Get current line content up to cursor as prefix */
	prefix_len = rl_point;
	if (prefix_len > 0)
	{
		prefix = strndup(rl_line_buffer, prefix_len);
		if (prefix)
		{
			/* Search backward for matching prefix */
			ret = history_search_prefix(prefix, -1);
			free(prefix);
			if (ret == 0)
			{
				/* Found a match - update line buffer */
				rl_replace_line(current_history()->line, 0);
				rl_point = rl_end;
				rl_redisplay();
				return (0);
			}
		}
	}
	/* No prefix or no match - use default previous-history */
	return (rl_get_previous_history(1, key));
}

/*
** Custom history search function for down arrow.
** If there's text before the cursor, search for history entries
** that start with that prefix. Otherwise, just go to next entry.
*/
static int	history_search_forward_prefix(int count, int key)
{
	int		ret;
	char	*prefix;
	int		prefix_len;

	(void)key;
	(void)count;
	/* Get current line content up to cursor as prefix */
	prefix_len = rl_point;
	if (prefix_len > 0)
	{
		prefix = strndup(rl_line_buffer, prefix_len);
		if (prefix)
		{
			/* Search forward for matching prefix */
			ret = history_search_prefix(prefix, 1);
			free(prefix);
			if (ret == 0)
			{
				/* Found a match - update line buffer */
				rl_replace_line(current_history()->line, 0);
				rl_point = rl_end;
				rl_redisplay();
				return (0);
			}
		}
	}
	/* No prefix or no match - use default next-history */
	return (rl_get_next_history(1, key));
}

/*
** Setup readline keybindings for prefix-based history search.
** This makes arrow keys behave like zsh - if you type a prefix
** and press up/down, it searches for commands starting with that prefix.
*/
void	setup_history_search(void)
{
	/* Bind Up arrow to our prefix search function */
	rl_bind_keyseq("\e[A", history_search_backward_prefix);
	rl_bind_keyseq("\eOA", history_search_backward_prefix);
	/* Bind Down arrow to our prefix search function */
	rl_bind_keyseq("\e[B", history_search_forward_prefix);
	rl_bind_keyseq("\eOB", history_search_forward_prefix);
	/* Enable history expansion (! syntax) */
	rl_variable_bind("history-preserve-point", "on");
	/* Make tab completion case-insensitive (optional, nice to have) */
	rl_variable_bind("completion-ignore-case", "on");
}
