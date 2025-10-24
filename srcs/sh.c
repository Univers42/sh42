/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:43:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/24 15:18:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
	bool    run = true;
	char    *line;

	while (run)
	{
		line = readline("hello >");
		if (line == NULL)
			break ;
		if (line[0] != '\0')
			add_history(line);
		printf("process: %s\n", line);
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
	printf("clear history\n");
	rl_clear_history();dd
	return (0);
}
