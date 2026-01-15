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

/*
** Setup readline for standard history navigation.
** We use the default arrow key bindings which work correctly.
*/
void	setup_history_search(void)
{
	/* Use standard history navigation - no custom bindings needed.
	   The default up/down arrow keys will navigate history normally. */
	
	/* Enable history expansion (! syntax) - handled by our own code */
	rl_variable_bind("history-preserve-point", "on");
}
