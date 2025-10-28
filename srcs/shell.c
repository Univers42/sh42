/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:43:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/28 14:02:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

int main(void)
{
	bool		run;
	char		*line;
	int			debug;
	t_scanner	*scanner;

	debug = 1;
	run = true;
	while (run)
	{
		line = readline("$>");
		if (line == NULL)
			break;
		if (line[0] != '\0')
			add_history(line);
		scanner = init_scanner(line);
		scan_all_tokens(scanner, debug);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		if (ft_strcmp(line, "history") == 0)
			rl_redisplay();
		free(line);
	}
	printf("clear hist\n");
	rl_clear_history();
	return (0);
}
