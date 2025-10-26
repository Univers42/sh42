/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:43:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/26 15:10:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

int main(void)
{
	bool run = true;
	char *line;
	int debug = 1; // set to 1 for debug mode, 0 for normal

	while (run)
	{
		line = readline("hello >");
		if (line == NULL)
			break;
		if (line[0] != '\0')
			add_history(line);

		t_scanner *scanner = init_scanner(line);
		scan_all_tokens(scanner, debug);

		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		if (strcmp(line, "history") == 0)
			rl_redisplay();
		free(line);
	}
	printf("clear history\n");
	rl_clear_history();
	return (0);
}
